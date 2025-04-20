#include <ADUC841.h>

// Upid ile kontrol_PID_int karsilastirmasi
////////////////// TANIMLAR ////////////////////////////////////////////

//sbit LED = P0^1;
//sbit led_ayar = P2^3;
//sbit buton_2= P2^1;

void timer_ayar(void);
void kesme_ayar(void);
void ADC_ayar(void);
void DAC_ayar(void);
void hata_hesapla(void);
void timer_baslat(void);
//void send_data(unsigned char c);
void DAC0_yaz(int deger);
//void seri_port_ayar(void);
int PID_hesaplama(int ref, int cikis);

unsigned char ref_h, ref_l, cikis_h, cikis_l,hata_h, hata_l, kontrol_PID_int_h, kontrol_PID_int_l;
unsigned int ref, cikis;
unsigned int ref_oku(void);
unsigned int cikis_oku(void);
int hata, kontrol_PID_int, onceki_hata = 0, toplam_hata = 0;
float kontrol_P, kontrol_I, kontrol_D, kontrol_PID;

unsigned int dongu_sayisi = 0; 

///// PID Kazançlari
float Kp = 0.1142;
float Ki = 0.1246;
float Kd = -4.8606;
float inter_up_katsayisi;
float fark_alma;
float inter_up_sonucu_hata;
//////////////////// TMR KESMESI //////////////////////////////////////////

void Timer0_kesmesi(void) interrupt 1 
{ 
    
    dongu_sayisi++;

    if (dongu_sayisi >=10) 
    { 
        //LED = ~LED; 
			  
				ref = ref_oku();
        ref_h = (ref >> 8) & 0xFF;
        ref_l = ref & 0xFF;
        
				cikis = cikis_oku();
        cikis_h = (cikis >> 8) & 0xFF;
        cikis_l = cikis & 0xFF;
			
			  kontrol_PID_int=PID_hesaplama(ref,cikis);        
        DAC0_yaz(kontrol_PID_int);

//        send_data('s');
//        send_data(ref_l);    
//        send_data(ref_h);
//        send_data(cikis_l);    
//        send_data(cikis_h);
//				send_data(hata_l);    
//        send_data(hata_h);
//				send_data(kontrol_PID_int_l);	
//				send_data(kontrol_PID_int_h);
//        send_data('e');

        dongu_sayisi = 0;
    }

    TH0 = 0xD4;
    TL0 = 0xCC;
    TF0 = 0;
}
///////////////// TMR kesme sonu //////////////////////////////////////////////////////
unsigned int ref_oku(void) 
{
    unsigned int ref;
    ADCCON2 = 0x07;  //adc0 kanalini ayarladik
    SCONV = 1;  //çevrim baslar
    while (SCONV == 1); //çevrim bitti mi soruyoruz
    ref = ((ADCDATAH & 0x0f) << 8) + ADCDATAL;  
    return ref;  
}    

unsigned int cikis_oku(void) 
{
    unsigned int out;
    ADCCON2 = 0x02; //adc1 kanalini ayarladik
    SCONV = 1; // çevrimi baslat
    while (SCONV == 1);//çevrim bitti mi soruyoruz
    out = ((ADCDATAH & 0x0f) << 8) + ADCDATAL;   
    return out;  
}

void DAC0_yaz(int deger)
{   
    if (deger < 0)
		{
			deger = 0;		
		} 
    if (deger > 4095)
		{
			deger = 4095;		
		} 

    DAC0H = ((deger >> 8) & 0x000F);
    DAC0L = deger;
}
//////////////////////////////////ana menü /////////////////////////////////
void main(void)
{
		//while(buton_2==1){};  // matlab ve keil senkron gitmesi için buton ile çalistirilacak
//    led_ayar = 0;
//    P0 = 0x00;

    timer_ayar();
    kesme_ayar();
    ADC_ayar();  
    DAC_ayar();
    timer_baslat();
//    seri_port_ayar();

  //  TR1 = 1; //seri port için tmr baslat

    while (1){};
}

void timer_ayar(void)
{
    TMOD = 0x01;
    TH0 = 0xD4;
    TL0 = 0xCC;
}

void kesme_ayar(void)
{
    ET0 = 1;
    EA = 1;
}

void ADC_ayar(void)
{
    ADCCON1 = 0xFC;
}

void DAC_ayar(void)
{
    DACCON = 0x7F;
}

void timer_baslat(void)
{
    TR0 = 1;
}

//void send_data(unsigned char c)
//{
//    SBUF = c;
//    while (TI == 0);
//    TI = 0;
//}

//void seri_port_ayar(void)
//{
//    TMOD = 0x20;// oto yuklemeli
//    SCON = 0x50;
//    TH1 = 0xDC; //boudrate ayari 9600 olasi için yuklenecek deger
//}
int PID_hesaplama(int ref, int cikis)
{  
   hata = ref - cikis;
			  hata_h=(hata >> 8)& 0xFF;
				hata_l= hata & 0xFF;
	
	
	toplam_hata += hata;
        kontrol_P = Kp * hata;
        kontrol_I = Ki * toplam_hata;
        kontrol_D = Kd * (hata - onceki_hata);
        kontrol_PID = kontrol_P + kontrol_I + kontrol_D; //sinirlandirilmis PID
				if(kontrol_PID < 0)
				{ kontrol_PID=0;	}
				if(kontrol_PID > 4095)
				{	kontrol_PID=4095;	}
				
  kontrol_PID_int=(int)kontrol_PID;  //sinirlandirilmamis PID
				kontrol_PID_int_h=(kontrol_PID_int >> 8) & 0xFF;
				kontrol_PID_int_l= kontrol_PID_int & 0xFF;
        onceki_hata = hata;
				
	inter_up_katsayisi=Ki/Kd*4.35;
	fark_alma=kontrol_PID-kontrol_PID_int;
	inter_up_sonucu_hata=ref+fark_alma*inter_up_katsayisi;
				
				
				
				return kontrol_PID_int;

}
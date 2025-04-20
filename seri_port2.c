#include <ADUC841.h>
////// tanimlamalar /////
void seri_port_ayar(void);

////// islemler ////////////

void main(void)
{	

	seri_port_ayar();
	TR1=1;  // tmr baslat

	while(1)
	{
			unsigned int a=4090;
	a_h=(a >> 8);
	a_l= a;
		send_data('s');
		send_data(a_h);	
		send_data(a_l);
		send_data('e');
		
	}
	
}

void send_data(unsigned char c)
{
			SBUF=c;
			while(TI==0); // veri giti mi bayragi kontrol ediliyor.
			TI=0;


}	

void seri_port_ayar(void)
{
	TMOD=0x20; // oto yuklemeli tmr1 mode2 0010 0000
	SCON=0x50; // mode_1 8 bit uart  (RI(geldi mi) TI(gitti mi) 0 0 1 mode_1(8bit))
	TH1=0xDC; // 9600 baud rate için hesaplanan deger

}
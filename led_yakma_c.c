#include <ADUC841.h>

////// P0.1 pinini yaktik /////////

sbit led=P2^3;   // bir portun tek bit erisimi icin sbit tanimi yazdik 
sbit lam=P0^1;

//void main()
//	{
//    while(1)
//			{
//			led=0;
//			P0=0x00;  // P0 tum pinlerini 0 yaptik
//			lam=1;
//			
//      }
//}

//////////////////////////////////////////////////
/////// yoklamali led yakip sondurme ////////////


//// Basit bir delay fonksiyonu
//void delay(unsigned int time) {
//    unsigned int i, j;
//    for (i = 0; i < time; i++) {
//        for (j = 0; j < 1275; j++); // Yaklasik 1 ms gecikme
//    }
//}

//void main() {
//    led = 0;     // LED sabit olarak her zaman 0 (LOW)
//	  P0=0x00;

//    while(1) {
//        lam = 0;     // Lam söner (LOW)
//        delay(500);  // 500 ms bekle

//        lam = 1;     // Lam yanar (HIGH)
//        delay(500);  // 500 ms bekle
//    }
//}
 
///////////////////////////////////////////////////////
////////// Timer Kesmeli  led yakma ////////////////

// Timer 0 kesmesi için gerekli tanimlar
void timer0_isr(void) interrupt 1 {
    static unsigned int count = 0;
    count++;

    if (count >= 500) { // 500 ms'lik gecikme
        lam = ~lam; // Lam durumunu tersle (toggle)
        count = 0;
    }
}

void main() {
    led = 0;     // LED sabit olarak her zaman 0 (LOW)
		P0=0x00;   // P0 portunun tüm pinlerini sifirlamak için kullanilir
    // Timer 0 ayarlari
    TMOD = 0x01;     // Timer 0 mod 1 (16-bit timer)
    TH0 = 0xFC;     // 1 ms'lik overflow için baslangiç degeri
    TL0 = 0x66;
    ET0 = 1;        // Timer 0 kesmesini aktif et
    EA = 1;         // Genel kesme izni
    TR0 = 1;        // Timer 0'i baslat

    while(1) {
        // Ana döngüde baska islemler yapilabilir
    }
}

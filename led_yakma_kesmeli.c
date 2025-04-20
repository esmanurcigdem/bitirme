#include <ADUC841.h>

//////// kesmeli 25 ms için tmr0 //////////////////

sbit LED = P0^1;  // LED P0.1 pinine tanimlandi
sbit led_ayar=P2^3;

void Timer0_Init(void) {
    TMOD = 0x01;      // Timer0 mod 1 (16-bit timer)
    TH0 = 0xD4;      // 65535 - 11059 = 54476 -> D4CC (hex)
    TL0 = 0xCC;
    ET0 = 1;         // Timer0 kesmesini aktif et
    EA = 1;          // Genel kesmeleri aktif et
    TR0 = 1;         // Timer0 baslat
}

void Timer0_kesmesi(void) interrupt 1 { // Timer0 kesme alt programi
    static unsigned int dongu_sayisi = 0; // 
    dongu_sayisi++;

    if (dongu_sayisi >=25) { 
        LED = ~LED;     // LED'i tersle
        dongu_sayisi = 0;
    }

    TH0 = 0xD4;        // Timer tekrar baslatilmasi için degerler
    TL0 = 0xCC;
    TF0 = 0;           // Timer0 bayragini temizle
}

void main(void) {
	  led_ayar=0;
    P0 = 0x00;       // P0 portunu sifirla
    Timer0_Init();  // Timer0 baslat

    while (1) {
        // Tüm islem kesme rutiniyle yapiliyor, burada baska islemler yapilabilir
    }
}

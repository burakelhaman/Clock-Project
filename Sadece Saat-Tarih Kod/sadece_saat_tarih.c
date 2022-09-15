#define     rtc_sclk   pin_c1
#define     rtc_io     pin_c2       
#define     rtc_rst    pin_c0  

#include <16f877A.h> // özellikleri alýr
#fuses XT, NOWDT, NOPROTECT , NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD//özellikler
#use delay(clock=4000000) //osilatör hýzý ayarlanýyor  
#include <math.h>
#include "ds1302.c" //ds1302'ye ait fonksiyonlar programa eklendi

#define display_1 pin_d0
#define display_2 pin_d1
#define display_3 pin_d2
#define display_4 pin_d3
#define display_5 pin_d4
#define display_6 pin_d5

const int digit[12]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x1C,0x46};//ortak anot
int16 i;
unsigned int bilgi;
int birler,onlar,saniye,dakika,saat,gun,ay,yil,haftanin_gunu,saniyeon,dakikaon,saaton,gunon,ayon,yilon,saniyebir,dakikabir,saatbir,gunbir,aybir,yilbir;


void gecis_animasyon()
{
output_d(0x00);
delay_ms(300); 

output_d(0b00000001);         
output_b( 0xb6 );    
delay_ms(100);            
output_d(0b00000010);         
output_b( 0xb6 );    
delay_ms(100);
output_d(0b00000100);         
output_b( 0xb6 );    
delay_ms(100);
output_d(0b00001000);         
output_b( 0xb6 );    
delay_ms(100);
output_d(0b00010000);         
output_b( 0xb6 );    
delay_ms(100);
output_d(0b00100000);         
output_b( 0xb6 );    
delay_ms(100);         

output_d(0x00);
delay_ms(300);
}
   
   void main() //baþlangýç
{
   set_tris_b(0x00);
   set_tris_d(0x00);
   output_a(0x00);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);
   rtc_init();
   delay_ms(20);
   rtc_set_datetime(25,07,19,5,16,21);

   while(1)
   {     
   rtc_get_time(saat,dakika,saniye);
   rtc_get_date(gun,ay,yil,haftanin_gunu);
   saniyeon=saniye/10;
   saniyebir=saniye%10;
   dakikaon=dakika/10;
   dakikabir=dakika%10;
   saaton=saat/10;
   saatbir=saat%10;
   gunon=gun/10;
   gunbir=gun%10;
   ayon=ay/10;
   aybir=ay%10;
   yilon=yil/10;
   yilbir=yil%10;
   
            for(i=0;i<50;i++)
         {
         output_high(display_1);  // 1. display aktif durumda
         output_b(digit[saaton]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_1);   // 1. display pasif durumda
         
         output_high(display_2);  // 2. display aktif durumda
         output_b(digit[saatbir]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_2);   // 2. display pasif durumda
   
         output_high(display_3);  //3. display aktif durumda
         output_b(digit[dakikaon]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_3);   // 3. display pasif durumda
         
         output_high(display_4);  // 4. display aktif durumda
         output_b(digit[dakikabir]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_4);   // 4. display pasif durumda
         
         output_high(display_5);  // 5. display aktif durumda
         output_b(digit[saniyeon]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_5);   // 5. display pasif durumda
         
         output_high(display_6);  // 6. display aktif durumda
         output_b(digit[saniyebir]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_6);   // 6. display pasif durumda
         } 
         
           gecis_animasyon();    
         
        for(i=0;i<50;i++)
         {
         output_high(display_1);  // 1. display aktif durumda
         output_b(digit[gunon]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_1);   // 1. display pasif durumda
         
         output_high(display_2);  // 2. display aktif durumda
         output_b(digit[gunbir]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_2);   // 2. display pasif durumda
   
         output_high(display_3);  //3. display aktif durumda
         output_b(digit[ayon]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_3);   // 3. display pasif durumda
         
         output_high(display_4);  // 4. display aktif durumda
         output_b(digit[aybir]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_4);   // 4. display pasif durumda
         
         output_high(display_5);  // 5. display aktif durumda
         output_b(digit[yilon]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_5);   // 5. display pasif durumda
         
         output_high(display_6);  // 6. display aktif durumda
         output_b(digit[yilbir]); // displaye bilgi gönderiliyor
         delay_ms(30);             // gecikme veriliyor
         output_low(display_6);   // 6. display pasif durumda
         }

   }
}

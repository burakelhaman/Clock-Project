 #include <16f877A.h> // özellikleri alýr
#fuses XT, NOWDT, NOPROTECT , NOBROWNOUT, NOLVP, NOPUT, NOWRT, NODEBUG, NOCPD//özellikler
#use delay(clock=4000000) //osilatör hýzý ayarlanýyor  
#include <math.h>
#include "ds18b20.c" //ds18b20'ye ait fonksiyonlar programa eklendi

#define display_1 pin_d0
#define display_2 pin_d1
#define display_3 pin_d2
#define display_4 pin_d3
#define display_5 pin_d4
#define display_6 pin_d5

const int digit[12]={0x40,0xF9,0xA4,0xB0,0b00001001,0x92,0x82,0xF8,0x80,0x90,0x1C,0x46};//ortak anot              01000000
int16 i;
unsigned long int bilgi;
int birler,onlar;

   void basamak_hesapla(){
   birler=bilgi%10;
   onlar=bilgi/10;
   }

void gecis_animasyon()
{
output_d(0x00);
delay_ms(100); 

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
delay_ms(100);
}
   
   void main() //baþlangýç
{
   set_tris_b(0x00);
   set_tris_d(0x00);
   output_a(0x00);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);

   while(1)
   {     
   bilgi=ds1820_read();
   basamak_hesapla();
   
           gecis_animasyon();    
           
      for(i=0;i<40;i++)
      {
      output_high(display_2);  // 2. display aktif durumda
      output_b(digit[onlar]); // displaye bilgi gönderiliyor
      delay_ms(30);             // gecikme veriliyor
      output_low(display_2);   // 2. display pasif durumda
   
      output_high(display_3);  //3. display aktif durumda
      output_b(digit[birler]); // displaye bilgi gönderiliyor
      delay_ms(30);             // gecikme veriliyor
      output_low(display_3);   // 3. display pasif durumda
         
      output_high(display_4);  // 4. display aktif durumda
      output_b(digit[10]); // displaye bilgi gönderiliyor
      delay_ms(30);             // gecikme veriliyor
      output_low(display_4);   // 4. display pasif durumda
         
      output_high(display_5);  // 5. display aktif durumda
      output_b(digit[11]); // displaye bilgi gönderiliyor
      delay_ms(30);             // gecikme veriliyor
      output_low(display_5);   // 5. display pasif durumda
      
      }
   }
}

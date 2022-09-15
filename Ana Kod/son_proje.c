#define SH_CP   PIN_b0 // shift register clock ucu
#define DS      PIN_b1 // data ucu
#define ST_CP   PIN_b2 // storage register ucu
#define     rtc_sclk   pin_c1
#define     rtc_io     pin_c2
#define     rtc_rst    pin_c0
#define     buzzer      pin_b4

#include <16f877A.h> // özellikleri alýr
#fuses XT, NOWDT, NOPROTECT
#use delay(clock=4000000) //osilatör hýzý ayarlanýyor  
#include <math.h>
#include "ds18b20.c" //ds18b20'ye ait fonksiyonlar programa eklendi
#include <DS1302.c> //ds1302'ye ait fonksiyonlar programa eklendi

#define set pin_c4
#define mod pin_c5
#define arttir pin_c6
#define azalt pin_c7
///////////////////////////////////////////////////////////////////////////////
byte saniye,dakika,saat,gun,ay,yil,haftanin_gunu;
char display1_onlar,display1_birler,display2_onlar,display2_birler,display3_onlar,display3_birler; 

const int digit[10]={0x77,0x41,0x3B,0x5B,0x4D,0x5E,0x7E,0x43,0x7F,0x5F};//ortak anot
const int digit1[10]={0xF7,0xC1,0xBB,0xDB,0xCD,0xDE,0xFE,0xC3,0xFF,0xDF};//ortak anot
unsigned int i,j,k;  // döngü sayacý
unsigned int maske=0x80;   // gönderilecek bilginin bitlerini öðrenmek için kullanýlan maske bilgisi.
int alarm_sira;
int set_degeri=0; // 1=saat 2=tarih
int mod_degeri=0; // 1=saat,gun 2=dakika,ay 3=saniye,yýl
unsigned long int bilgi; //sýcaklýk bilgisi
int sicaklik_ekrani=0; //1=aktif, 0=pasif   
///////////////////////////////////////////////////////////////////////////////
void display_goster(unsigned char bilgi)
{
   for (i=0;i<8;i++)
   {
      if(bilgi&maske) output_high(DS); //bilginin ilk biti 1 ise seri bilginin ilk biti olarak 1 ver.

      else output_low(DS);
      maske>>=1;  //Maskeyi ikinci-üçüncü... bite ayarlamak için kaydýr.
      output_high(SH_CP); // shift register clock giriþini ver.
      delay_us(2);
      output_low(SH_CP);// shift register clock giriþini kapa.
      delay_ms(1);
    }
    maske=0x80; //maskeyi eski haline getir.
}//////////////////////////////////////////////////////////////////////////////
void ekran()
{
   display_goster(digit[display3_birler]);
   display_goster(digit[display3_onlar]);
   display_goster(digit[display2_birler]);
   display_goster(digit[display2_onlar]);
   display_goster(digit[display1_birler]);
   display_goster(digit[display1_onlar]);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(5);
}//////////////////////////////////////////////////////////////////////////////
void gecis_animasyon()
{
   display_goster(0x1A);
   display_goster(0x1A);
   display_goster(0x1A);
   display_goster(0x1A);
   display_goster(0x1A);
   display_goster(0x1A);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(2);
   output_low(ST_CP);
   delay_ms(50);
}//////////////////////////////////////////////////////////////////////////////
void saat_basi()
{
   if ((dakika==0)&(saniye==0))
   {
   output_high(buzzer);delay_ms(200);  output_low(buzzer);delay_ms(200);
   }
}//////////////////////////////////////////////////////////////////////////////
void display1_goster()
{
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(digit[display1_birler]);
   display_goster(digit[display1_onlar]);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(150);
       
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(5);
}//////////////////////////////////////////////////////////////////////////////
void display2_goster()
{
   display_goster(0x00);
   display_goster(0x00);
   display_goster(digit[display2_birler]);
   display_goster(digit[display2_onlar]);
   display_goster(0x00);
   display_goster(0x00);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(150);
       
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün
   delay_us(30);
   output_low(ST_CP);
   delay_ms(5);
}//////////////////////////////////////////////////////////////////////////////
void display3_goster()
{
   display_goster(digit[display3_birler]);
   display_goster(digit[display3_onlar]);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(150);
       
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(5);
}//////////////////////////////////////////////////////////////////////////////
void set_konumunda_display_goster() 
{
delay_ms(200);
   if(sicaklik_ekrani==1)
   {
   display_goster(display3_birler);
   display_goster(display3_onlar);
   display_goster(display2_birler);
   display_goster(digit[display2_onlar]);
   display_goster(digit[display1_birler]);
   display_goster(display1_onlar);
   }
   
   else
   {
   display_goster(digit[display3_birler]);
   display_goster(digit[display3_onlar]);
   display_goster(digit[display2_birler]);
   display_goster(digit[display2_onlar]);
   display_goster(digit[display1_birler]);
   display_goster(digit[display1_onlar]);
   }
   
   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(150);
       
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);
   display_goster(0x00);

   output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
   delay_us(30);
   output_low(ST_CP);
   delay_ms(5);

}//////////////////////////////////////////////////////////////////////////////
void alarm_kontrol(){
   for(i=1;i<=8;i++)
   {
      if(read_eeprom((3*i)-2)==saat && read_eeprom((3*i)-1)==dakika && read_eeprom(3*i)==saniye && saat!=0 && dakika!=0)
      {
         for(k=0;k<=4;k++)
         {
            ekran();
            output_high(buzzer);delay_ms(200);  output_low(buzzer);delay_ms(200);
            output_high(buzzer);delay_ms(200);  output_low(buzzer);delay_ms(200);
            output_high(buzzer);delay_ms(200);  output_low(buzzer);delay_ms(200);
            output_high(buzzer);delay_ms(200);  output_low(buzzer);delay_ms(200);
         }
      }
   }
}//////////////////////////////////////////////////////////////////////////////
void buton()
{
   output_high(buzzer);delay_ms(150);  output_low(buzzer); break;
}//////////////////////////////////////////////////////////////////////////////
void alarm_secim(){
   while(set_degeri==1)
   { saat=1;
      for(;;)               
      {
         while(input(arttir) && input(azalt))
         {
            display1_birler=saat%10;             
            display1_onlar=saat/10;            
            display1_goster();
         }
         
         display1_birler=saat%10;             
         display1_onlar=saat/10;            
         display1_goster();
         
         if(input(arttir)){buton(); saat++; while(input(arttir)){display1_goster();} if(saat==9){saat=1;} }
         if(input(azalt)){buton(); saat--; while(input(azalt)){display1_goster();} if(saat==0){saat=8;} }
         if(input(set)){buton(); while(input(set)){alarm_sira=saat;} goto alarmsecim_cik;}         
         
         delay_ms(5);
      }
   }
      alarmsecim_cik:
      gecis_animasyon();
}///////////////////////////////////////////////////////////////////////////////////3x-2,3x-1,3x
void alarm_set()
{
basla:
   set_degeri=1;
   alarm_secim();
   
   while(set_degeri==1)
   {
      saat=read_eeprom(3*alarm_sira-2); dakika=read_eeprom(3*alarm_sira-1); saniye=read_eeprom(3*alarm_sira);
      display1_birler=saat%10;
      display1_onlar=saat/10;
      display2_birler=dakika%10;
      display2_onlar=dakika/10;
      display3_birler=saniye%10;
      display3_onlar=saniye/10;
      set_konumunda_display_goster();
   
      if(input(set)){buton(); while(input(set)){ekran();} goto alarmdan_cik; }
      if(input(mod)){buton(); while(input(mod)){ekran();} mod_degeri=1;}
      if(input(arttir) && input(azalt)){buton();saat=0;dakika=0;saniye=0; write_eeprom((3*alarm_sira-2),0); delay_ms(5); write_eeprom((3*alarm_sira-1),0); delay_ms(5); write_eeprom((3*alarm_sira),0); goto basla;}
   
      while(mod_degeri==1)
      { 
         for(;;)               
         {            
            display1_birler=saat%10;             
            display1_onlar=saat/10;            
            display1_goster();
                                                               
            if(input(arttir)){buton(); saat++; while(input(arttir)){display1_goster();} if(saat==24){saat=0;} }
            if(input(azalt)){buton();saat--; while(input(azalt)){display1_goster();} if(saat==-1){saat=23;} }
            if(input(mod)){buton(); while(input(mod)){display1_goster();} mod_degeri=2; goto mod1_cik;}                                    
            write_eeprom((3*alarm_sira-2),saat);
            delay_ms(5);
         }
      } 
      mod1_cik:

      while(mod_degeri==2)
      {                              
         for(;;)               
         {                      
            display2_birler=dakika%10;             
            display2_onlar=dakika/10;            
            display2_goster();
  
            if(input(arttir)){buton(); dakika++; while(input(arttir)){display2_goster();} if(dakika==60){dakika=0;} }
            if(input(azalt)){buton(); dakika--; while(input(azalt)){display2_goster();} if(dakika==-1){dakika=59;} } 
            if(input(mod)){buton(); while(input(mod)){display2_goster();} mod_degeri=3; goto mod2_cik;}                                    
            write_eeprom((3*alarm_sira-1),dakika);
            delay_ms(5);
         } 
      } 
      mod2_cik:

      while(mod_degeri==3)
      {   
    
         for(;;)
         {  
             display3_birler=saniye%10;             
             display3_onlar=saniye/10;            
             display3_goster();
                                      
             if(input(arttir)){buton(); saniye++; while(input(arttir)){display3_goster();} if(saniye==60){saniye=0;} }
             if(input(azalt)){buton(); saniye--; while(input(azalt)){display3_goster();} if(saniye==-1){saniye=59;} }
             if(input(mod)){buton(); while(input(mod)){display3_goster();} mod_degeri=0; goto alarmdan_cik;}
             write_eeprom((3*alarm_sira),saniye);
             delay_ms(5);
         }
      }            
         delay_us(1);
   }

   alarmdan_cik:
   gecis_animasyon();
}//////////////////////////////////////////////////////////////////////////////
void ayarlar()
{
   if (input(set))
   {
      while(input(set))
      {
         ekran();
      }
      set_degeri=1;
  }
  else { goto ayardan_cik; }

   while(set_degeri==1) //saat ayarý            
   {
      rtc_get_time(saat,dakika,saniye);
      display1_birler=saat%10;
      display1_onlar=saat/10;
      display2_birler= dakika%10;
      display2_onlar=dakika/10;
      display3_birler=saniye%10;
      display3_onlar=saniye/10;
      set_konumunda_display_goster();

      if(input(set)){buton();  while(input(set)){ekran();} set_degeri=2; goto set1_den_cik; }              
      if(input(mod)){buton();  while(input(mod)){ekran();} mod_degeri=1; }
                         
      while(mod_degeri==1)   //saat ayarlanýyor.
      {    
         for(;;)               
         {            
            display1_birler=saat%10;             
            display1_onlar=saat/10;            
            display1_goster();
                                                                     
            if(input(arttir)){buton(); saat++; while(input(arttir)){display1_goster();} if(saat==24){saat=0;} }
            if(input(azalt)){buton(); saat--; while(input(azalt)){display1_goster();} if(saat==-1){saat=23;} }
            if(input(mod)){buton(); while(input(mod)){display1_goster();} mod_degeri=2; goto mod1s_cik;}                                    
            write_ds1302(0x84,get_bcd(saat)); //saat degeri gonder. 0-23 
            delay_ms(5);
         } //for smod1 
      } //while mod deger = 1
mod1s_cik:
                           
      while(mod_degeri==2)   //dakika ayarlanýyor.
      {                              
         for(;;)               
         {                      
            display2_birler=dakika%10;             
            display2_onlar=dakika/10;            
            display2_goster();
        
            if(input(arttir)){buton(); dakika++; while(input(arttir)){display2_goster();} if(dakika==60){dakika=0;} }
            if(input(azalt)){buton(); dakika--; while(input(azalt)){display2_goster();} if(dakika==-1){dakika=59;} } 
            if(input(mod)){buton(); while(input(mod)){display2_goster();} mod_degeri=3; goto mod2s_cik;}                                    
            write_ds1302(0x82,get_bcd(dakika)); //dakika degeri gonder. 00-59  
            delay_ms(5);
         } //for smod2
      } //while mod deger = 2 
mod2s_cik:   

      while(mod_degeri==3)   //saniye ayarlanýyor.
      {   
         for(;;)
         {  
            display3_birler=saniye%10;             
            display3_onlar=saniye/10;            
            display3_goster();
                                      
            if(input(arttir)){buton(); saniye++; while(input(arttir)){display3_goster();} if(saniye==60){saniye=0;} }
            if(input(azalt)){buton(); saniye--; while(input(azalt)){display3_goster();} if(saniye==-1){saniye=59;} }
            if(input(mod)){buton(); while(input(mod)){display3_goster();} mod_degeri=0; goto mod3s_cik;}                                    
            write_ds1302(0x80,get_bcd(saniye)); //saniye degeri gonder. 00-59
            delay_ms(5);
         } //for smod3         
      } //while mod deger = 3 
mod3s_cik:
                           
      delay_us(1);
   } // while set deger = 1 
set1_den_cik:
gecis_animasyon();

   while(set_degeri==2) //tarih ayarý            
   {

      rtc_get_date(gun,ay,yil,haftanin_gunu); 
      display1_birler=gun%10;                                              
      display1_onlar=gun/10;                                              
      display2_birler= ay%10;
      display2_onlar=ay/10;                                              
      display3_birler=yil%10;         
      display3_onlar=yil/10;  
      set_konumunda_display_goster();                                               

      if(input(set)){buton();  while(input(set)){ekran();} set_degeri=0; goto set2_den_cik; }                                                             
      if(input(mod)){buton(); while(input(mod)){ekran();} mod_degeri=1; }
                         
         while(mod_degeri==1)  //gun ayarlanýyor.
         { 
            for(;;)               
            {            
               display1_birler=gun%10;             
               display1_onlar=gun/10;            
               display1_goster();
                                                               
               if(input(arttir)){buton(); gun++; while(input(arttir)){display1_goster();} if(gun==32){gun=1;} }
               if(input(azalt)){buton(); gun--; while(input(azalt)){display1_goster();} if(gun==0){gun=31;} }
               if(input(mod)){buton(); while(input(mod)){display1_goster();} mod_degeri=2; goto mod1t_cik;}                                    
               write_ds1302(0x86,get_bcd(gun)); //gun degeri gonder. 1-31 
               delay_ms(5);
            } //for tmod1 
         } //while mod deger = 1
mod1t_cik:                   
                           
         while(mod_degeri==2)    //ay ayarlanýyor.
         { 
            for(;;)               
            {                      
               display2_birler=ay%10;             
               display2_onlar=ay/10;            
               display2_goster();
  
               if(input(arttir)){buton(); ay++; while(input(arttir)){display2_goster();} if(ay==13){ay=1;} }
               if(input(azalt)){buton(); ay--; while(input(azalt)){display2_goster();} if(ay==0){ay=12;} } 
               if(input(mod)){buton(); while(input(mod)){display2_goster();} mod_degeri=3; goto mod2t_cik;}                                    
               write_ds1302(0x88,get_bcd(ay)); //ay degeri gonder. 1-12
               delay_ms(5);
            } //for tmod2 
         } //while mod deger = 2 
mod2t_cik:

         while(mod_degeri==3)  //yil ayarlanýyor.
         {    
            for(;;)
            {  
               display3_birler=yil%10;             
               display3_onlar=yil/10;            
               display3_goster();
                                      
               if(input(arttir)){buton(); yil++; while(input(arttir)){display3_goster();} if(yil==100){yil=0;} }
               if(input(azalt)){buton(); yil--; while(input(azalt)){display3_goster();} if(yil==-1){yil=99;} }
               if(input(mod)){buton(); while(input(mod)){display3_goster();} mod_degeri=0; goto mod3t_cik;}                                    
               write_ds1302(0x8C,get_bcd(yil)); //yil degeri gonder. 00-99
               delay_ms(5);                   
            } //for tmod3        
         } //while mod deger = 3 
mod3t_cik:

      delay_us(1);
   } // while set deger = 2 
set2_den_cik:
gecis_animasyon();

ayardan_cik:
output_e(0b000); //lambalar söndü
} // void ayar 
///////////////////////////////////////////////////////////////////////////////
void main() //baþlangýç
{
   output_a(0x00);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);
   output_e(0x00);
   
   rtc_init();
   delay_ms(20);

   
      while(1)
      {
         for(j=0;j<190;j++) //for saat
         {
            if (input(set)){buton(); ayarlar();}
            if (input(arttir) && input(azalt)){buton(); alarm_set();}
       
            rtc_get_time(saat,dakika,saniye);  // DS1302'den saat,dakika,saniye bilgileri alýnýyor
            saat_basi();
            alarm_kontrol();
       
            display1_birler=saat%10;  // birler basamaðý için % iþareti ile mod lu bölme yapýlarak 10 a bölümünden kalan hesaplanýyor.             
            display1_onlar=saat/10;   // saat degerini 10 a bölüp onlar basamaðý hesaplanýyor.
            display2_birler= dakika%10;
            display2_onlar=dakika/10;
            display3_birler=saniye%10;    
            display3_onlar=saniye/10;
       
            if(saniye%2==1)
            {
               display_goster(digit1[display3_birler]);
               display_goster(digit1[display3_onlar]);
               display_goster(digit1[display2_birler]);
               display_goster(digit1[display2_onlar]);
               display_goster(digit1[display1_birler]);
               display_goster(digit1[display1_onlar]);
            }
            else
            {
               display_goster(digit[display3_birler]);
               display_goster(digit[display3_onlar]);
               display_goster(digit[display2_birler]);
               display_goster(digit[display2_onlar]);
               display_goster(digit[display1_birler]);
               display_goster(digit[display1_onlar]);
            }
            
            output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
            delay_us(30);
            output_low(ST_CP);
            delay_ms(6);    
            } // for saat  
       
       gecis_animasyon();
       
         for(j=0;j<190;j++)  //for tarih
         {
         if (input(set)) {buton(); ayarlar();}
         if (input(arttir) && input(azalt)){buton(); alarm_set();}
       
         rtc_get_time(saat,dakika,saniye);  // DS1302'den saat,dakika,saniye bilgileri alýnýyor
         saat_basi();
         alarm_kontrol();

         rtc_get_date(gun,ay,yil,haftanin_gunu);
         display1_onlar=gun/10;   // saat degerini 10 a bölüp onlar basamaðý hesaplanýyor.
         display1_birler=gun%10;  // birler basamaðý için % iþareti ile mod lu bölme yapýlarak 10 a bölümünden kalan hesaplanýyor.             
         display2_onlar=ay/10;
         display2_birler= ay%10;
         display3_onlar=yil/10; 
         display3_birler=yil%10;      
       
         display_goster(digit1[display3_birler]);
         display_goster(digit1[display3_onlar]);
         display_goster(digit1[display2_birler]);
         display_goster(digit1[display2_onlar]);
         display_goster(digit1[display1_birler]);
         display_goster(digit1[display1_onlar]);

         output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
         delay_us(30);
         output_low(ST_CP);
         delay_ms(6);           
       } // for tarih 
       
       gecis_animasyon(); 
       sicaklik_ekrani=1;
       bilgi=ds1820_read()+2;
       for(j=0;j<190;j++)
       {
         if (input(set)) {buton(); ayarlar();}
         if (input(arttir) && input(azalt)){buton(); alarm_set();}
         
         rtc_get_time(saat,dakika,saniye);  // DS1302'den saat,dakika,saniye bilgileri alýnýyor
         saat_basi();
         alarm_kontrol();
         display3_birler=(0X00);
         display3_onlar=(0xB6);
         display2_birler=(0x0F);
         display1_birler=(int)bilgi/10;         
         display2_onlar=(int)bilgi%10;
         display1_onlar=(0X00);
      
         display_goster(display3_birler);
         display_goster(display3_onlar);
         display_goster(display2_birler);
         display_goster(digit[display2_onlar]);
         display_goster(digit[display1_birler]);
         display_goster(display1_onlar);

         output_high(ST_CP);  //storage register clock giriþini ver ve bilgiler portlarda görünsün.
         delay_us(30);
         output_low(ST_CP);
         delay_ms(6);       
       }
       sicaklik_ekrani=0;
       gecis_animasyon();
   }
}

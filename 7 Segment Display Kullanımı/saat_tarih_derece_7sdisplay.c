/*
Proje Adý: 7 SEGMENT DÝSPLAY ÝLE SAAT-TARÝH-SICAKLIK GÖSTERGESÝ

- 16F877
- DS1302
- LM35
*/               

//----- ds1302 pinleri tanýmlanýyor --------------------------------------------
#ifndef     rtc_sclk
#define     rtc_sclk   pin_c1
#define     rtc_io     pin_c2       
#define     rtc_rst    pin_c0

#endif                        
//------------------------------------------------------------------------------
                     
#include <16f877.h>                              
#device adc=10
#fuses XT,NOWDT,NOPROTECT    
#use delay (clock=20000000)
#include <math.h>
#include <DS1302.c>                            

#define set pin_c4
#define mod pin_c5
#define arttir pin_c6
#define azalt pin_c7
 
byte saniye,dakika,saat,gun,ay,yil,haftanin_gunu;
                 
char display1_onlar,display1_birler,display2_onlar,display2_birler,display3_onlar,display3_birler;  
                  
const int display_digit[10]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};  // ortak anotlu için              
int i;
int set_degeri=0; // 1=saat 2=tarih
int mod_degeri=0; // 1=saat,gun 2=dakika,ay 3=saniye,yýl  
unsigned long int sicaklik_bilgisi;     
float sicaklik_degeri,sicaklik_voltaj;     
int sicaklik_ekrani=0; //1=aktif, 0=pasif   
           
//------------------------------------------------------------------------------

//----- displaylerin hepsi taranýyor -------------------------------------------
void display_tara()                
{
                                 
output_d(0b00000001);       
output_b(display_digit[display1_onlar]);    
delay_ms(2);            
output_d(0b00000010);              
output_b(display_digit[display1_birler]);      
delay_ms(2);                
output_d(0b00000100);
output_b(display_digit[display2_onlar]);     
delay_ms(2);            
output_d(0b00001000);
output_b(display_digit[display2_birler]);     
delay_ms(2); 
output_d(0b00010000);
output_b(display_digit[display3_onlar]);     
delay_ms(2);
output_d(0b00100000);
output_b(display_digit[display3_birler]);     
delay_ms(2);

} // display tara  
//------------------------------------------------------------------------------ 

//-------- sicaklik gosterimi icin tarama --------------------------------------
void sicaklik_tara()
{

output_d(0b00000001);       
output_b(display_digit[display1_onlar]);    
delay_ms(2);            
output_d(0b00000010);                   
output_b(display_digit[display1_birler]);     
delay_ms(2);                
output_d(0b00000100);
output_b(0x9c); // derece simgesi    
delay_ms(2);            
output_d(0b00001000);
output_b(0xc6); // C harfi   
delay_ms(2); 
output_d(0b00000000);

}
//------------------------------------------------------------------------------
 
//------- gecisler icin animasyon ----------------------------------------------
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
//------------------------------------------------------------------------------ 

//------- sadece display1 taranýyor --------------------------------------------
void display1_tara()
{
output_d(0b00000001);       
output_b(display_digit[display1_onlar]);    
delay_ms(2);            
output_d(0b00000010);              
output_b(display_digit[display1_birler]);     
delay_ms(2);
}
//------------------------------------------------------------------------------

//------- sadece display2 taranýyor --------------------------------------------
void display2_tara()
{
output_d(0b00000100);
output_b(display_digit[display2_onlar]);     
delay_ms(2);            
output_d(0b00001000);
output_b(display_digit[display2_birler]);     
delay_ms(2); 
} 
//------------------------------------------------------------------------------

//------- sadece display3 taranýyor --------------------------------------------
void display3_tara()
{
output_d(0b00010000);
output_b(display_digit[display3_onlar]);     
delay_ms(2);
output_d(0b00100000);
output_b(display_digit[display3_birler]);     
delay_ms(2);
}
//------------------------------------------------------------------------------

//------- SET konumunda displayler taranýyor -----------------------------------
void set_konumunda_display_tara() 
{
output_d(0x00);
delay_ms(300);
display_tara();
display_tara();
display_tara();
display_tara();
display_tara();
}
//------------------------------------------------------------------------------

//------------- saat tarih ayarý -----------------------------------------------
void ayarlar()
{ 

  if (input(set))
  {
   while(input(set))
   {
      if(sicaklik_ekrani==1){ sicaklik_tara(); }
      else { display_tara(); }
      output_low(pin_a1);output_low(pin_a2);
   }
  set_degeri=1;                                        
  }
  else { goto ayardan_cik; }

while(set_degeri==1) //saat ayarý            
{
output_e(0b001);

rtc_get_time(saat,dakika,saniye);
display1_birler=saat%10;             
display1_onlar=saat/10; 
display2_birler= dakika%10;
display2_onlar=dakika/10;
display3_birler=saniye%10; 
display3_onlar=saniye/10; 
set_konumunda_display_tara();

if(input(set)){ while(input(set)){display_tara();} set_degeri=2; goto set1_den_cik; }
                   
if(input(mod)){ while(input(mod)){display_tara();} mod_degeri=1; }
                         
 while(mod_degeri==1)   //saat ayarlanýyor.
 { 
      
  for(;;)               
  {            
    display1_birler=saat%10;             
    display1_onlar=saat/10;            
    display1_tara();
                                                               
 if(input(arttir)){saat++; while(input(arttir)){display1_tara();} if(saat==24){saat=0;} }
 if(input(azalt)){saat--; while(input(azalt)){display1_tara();} if(saat==-1){saat=23;} }
 if(input(mod)){while(input(mod)){display1_tara();} mod_degeri=2; goto mod1s_cik;}                                    
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
    display2_tara();
  
 if(input(arttir)){dakika++; while(input(arttir)){display2_tara();} if(dakika==60){dakika=0;} }
 if(input(azalt)){dakika--; while(input(azalt)){display2_tara();} if(dakika==-1){dakika=59;} } 
 if(input(mod)){while(input(mod)){display2_tara();} mod_degeri=3; goto mod2s_cik;}                                    
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
    display3_tara();
                                      
 if(input(arttir)){saniye++; while(input(arttir)){display3_tara();} if(saniye==60){saniye=0;} }
 if(input(azalt)){saniye--; while(input(azalt)){display3_tara();} if(saniye==-1){saniye=59;} }
 if(input(mod)){while(input(mod)){display3_tara();} mod_degeri=0; goto mod3s_cik;}                                    
 write_ds1302(0x80,get_bcd(saniye)); //saniye degeri gonder. 00-59
 delay_ms(5);
} //for smod3         
} //while mod deger = 3 
mod3s_cik:
                           
delay_us(1);
} // while set deger = 1 
set1_den_cik:


 while(set_degeri==2) //tarih ayarý            
{
output_e(0b010); 

rtc_get_date(gun,ay,yil,haftanin_gunu); 
display1_birler=gun%10;                                              
display1_onlar=gun/10;                                              
display2_birler= ay%10;
display2_onlar=ay/10;                                              
display3_birler=yil%10;         
display3_onlar=yil/10;  
set_konumunda_display_tara();                                               

if(input(set)){ while(input(set)){display_tara();} set_degeri=0; goto set2_den_cik; }
                                                               
if(input(mod)){ while(input(mod)){display_tara();} mod_degeri=1; }
                         
 while(mod_degeri==1)  //gun ayarlanýyor.
 { 
      
  for(;;)               
  {            
    display1_birler=gun%10;             
    display1_onlar=gun/10;            
    display1_tara();
                                                               
 if(input(arttir)){gun++; while(input(arttir)){display1_tara();} if(gun==32){gun=1;} }
 if(input(azalt)){gun--; while(input(azalt)){display1_tara();} if(gun==0){gun=31;} }
 if(input(mod)){while(input(mod)){display1_tara();} mod_degeri=2; goto mod1t_cik;}                                    
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
    display2_tara();
  
 if(input(arttir)){ay++; while(input(arttir)){display2_tara();} if(ay==13){ay=1;} }
 if(input(azalt)){ay--; while(input(azalt)){display2_tara();} if(ay==0){ay=12;} } 
 if(input(mod)){while(input(mod)){display2_tara();} mod_degeri=3; goto mod2t_cik;}                                    
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
    display3_tara();
                                      
 if(input(arttir)){yil++; while(input(arttir)){display3_tara();} if(yil==100){yil=0;} }
 if(input(azalt)){yil--; while(input(azalt)){display3_tara();} if(yil==-1){yil=99;} }
 if(input(mod)){while(input(mod)){display3_tara();} mod_degeri=0; goto mod3t_cik;}                                    
 write_ds1302(0x8C,get_bcd(yil)); //yil degeri gonder. 00-99
 delay_ms(5);                   
} //for tmod3        
} //while mod deger = 3 
mod3t_cik:

delay_us(1); 
} // while set deger = 2 
set2_den_cik:

ayardan_cik:
output_e(0b000); //lambalar söndü
} // void ayar 
//------------------------------------------------------------------------------             

//--------- saat basi alarm caliyor --------------------------------------------
void saat_basi_alarm()
{
if ((display2_onlar==0)&(display2_birler==0)&(display3_onlar==0)&(display3_birler==0))
{
output_high(pin_c3); delay_ms(100); output_low(pin_c3); delay_ms(100);
output_high(pin_c3); delay_ms(100); output_low(pin_c3);
}
}
//------------------------------------------------------------------------------

//------- ana program ----------------------------------------------------------
void main ()                                 
{  
   output_a(0x00);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);
   output_e(0x00);
   delay_ms(2);
   setup_adc(adc_clock_internal);   
   setup_adc_ports(AN0);
    
   rtc_init();  // DS1302 baþlangýç ayarlarý yapýlýyor
   delay_ms(20);
   //rtc_set_datetime(1,02,10,7,21,20); // Tarih ve Saat ayarlarý yapýlýyor
   
   for(;;)                
   {                           
       
       gecis_animasyon();

       output_low(pin_a2);
       output_high(pin_a1); 
       
       for(i=0;i<250;i++) //for saat
       {
       if (input(set)){ayarlar();output_low(pin_a2);output_high(pin_a1);}
       rtc_get_time(saat,dakika,saniye);       // DS1302'den saat,dakika,saniye bilgileri alýnýyor
       display1_birler=saat%10;  // birler basamaðý için % iþareti ile mod lu bölme yapýlarak 10 a bölümünden kalan hesaplanýyor.             
       display1_onlar=saat/10;   // saat degerini 10 a bölüp onlar basamaðý hesaplanýyor.
       display2_birler= dakika%10;
       display2_onlar=dakika/10;
       display3_birler=saniye%10;    
       display3_onlar=saniye/10; 
       display_tara();                  
       delay_ms(5);    
       } // for saat  
       
       saat_basi_alarm();
       
       output_low(pin_a1);
       output_low(pin_a2);
       
       gecis_animasyon();
 
       output_low(pin_a1);
       output_high(pin_a2);
       
       for(i=0;i<250;i++)  //for tarih
       {   
       if (input(set)){ayarlar();output_low(pin_a1);output_high(pin_a2);}
       rtc_get_date(gun,ay,yil,haftanin_gunu); 
       display1_birler=gun%10;  // birler basamaðý için % iþareti ile mod lu bölme yapýlarak 10 a bölümünden kalan hesaplanýyor.             
       display1_onlar=gun/10;   // saat degerini 10 a bölüp onlar basamaðý hesaplanýyor.
       display2_birler= ay%10;
       display2_onlar=ay/10;
       display3_birler=yil%10;         
       display3_onlar=yil/10; 
       display_tara();
       delay_ms(5);           
       } // for tarih 
       
       output_low(pin_a1);
       output_low(pin_a2);
       
       gecis_animasyon();
       
       sicaklik_ekrani=1;
       for(i=0;i<=250;i++) //for sicaklik
       {

       if (input(set)){ayarlar();}
       set_adc_channel(0);
       delay_us(20);                                                                                           
       sicaklik_bilgisi= read_adc();          
       sicaklik_voltaj=(0.0048828125*sicaklik_bilgisi)*1000;
       sicaklik_degeri=floor((sicaklik_voltaj/10)+2);    
       display1_birler=(int)sicaklik_degeri%10;
       display1_onlar=(int)(sicaklik_degeri/10);   
       sicaklik_tara();      
       delay_ms(5);
       }//for sicaklik                    
       sicaklik_ekrani=0;     
       
   } //for sonsuz  
}  // main 
                                         
//write_ds1302(0x84,get_bcd(saat)); //saat degeri gonder. 0-23
//write_ds1302(0x82,get_bcd(6)); //dakika degeri gonder. 00-59 
//write_ds1302(0x80,get_bcd(7)); //saniye degeri gonder. 00-59
 
//write_ds1302(0x86,get_bcd(7)); //gun degeri gonder. 1-31 
//write_ds1302(0x88,get_bcd(1)); //ay degeri gonder. 1-12
//write_ds1302(0x8a,get_bcd(3)); //haftanýn gün degeri gonder. 1-7
//write_ds1302(0x8c,get_bcd(9)); //yýl degeri gonder. 00-99    

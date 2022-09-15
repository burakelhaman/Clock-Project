#include <kesme.h>
#use fast_io(b) //Port yönlendirme komutlarý B portu için geçerli
int i;
#int_ext // Dýþ (External) RB0/INT kesmesi
void ext_kesmesi(){
output_high(pin_b1);
 delay_ms(1000);
 output_low(pin_b1);
 delay_ms(3000);
 
}

void main()
{

 set_tris_b(0x01); // RB0 pini giriþ,diðer uçlar çýkýþ olarak yönlendiriliyor
 output_b(0x00); // B portu çýkýþý ilk anda sýfýrlanýyor
 
 ext_int_edge(H_TO_L); // INT_EXT kesmesinin düþen kenarda aktif olacaðýný belirtir
 
 enable_interrupts(INT_EXT); // INT_EXT kesmesini aktif yapar
 enable_interrupts(GLOBAL); // Aktif edilen kesmelere izin ver
 
 while(1); // Sonsuz döngü
 }

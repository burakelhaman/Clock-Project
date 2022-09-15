#include <kesme.h>
#use fast_io(b) //Port y�nlendirme komutlar� B portu i�in ge�erli
int i;
#int_ext // D�� (External) RB0/INT kesmesi
void ext_kesmesi(){
output_high(pin_b1);
 delay_ms(1000);
 output_low(pin_b1);
 delay_ms(3000);
 
}

void main()
{

 set_tris_b(0x01); // RB0 pini giri�,di�er u�lar ��k�� olarak y�nlendiriliyor
 output_b(0x00); // B portu ��k��� ilk anda s�f�rlan�yor
 
 ext_int_edge(H_TO_L); // INT_EXT kesmesinin d��en kenarda aktif olaca��n� belirtir
 
 enable_interrupts(INT_EXT); // INT_EXT kesmesini aktif yapar
 enable_interrupts(GLOBAL); // Aktif edilen kesmelere izin ver
 
 while(1); // Sonsuz d�ng�
 }

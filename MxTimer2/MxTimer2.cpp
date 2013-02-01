/*
  MxTimer2.h - Using timer2 with a configurable resolution user 32.768k
  Fredqian <fuqian@smeshlink.com>
  Changed from FlexTimer2

 */
 
#include <MxTimer2.h>
unsigned long MxTimer2::time_units;
void (*MxTimer2::func)();
volatile unsigned long MxTimer2::count;
volatile char MxTimer2::overflowing;
//���÷ֱ��ʣ�resolution��ʾ1���ж϶��ٴΣ�units��ʾ��resolution�ֱ������жϴ�����f��ʾ�ж���//unit����Ҫִ�еĺ���
void MxTimer2::set(unsigned long units, unsigned int resolution, void (*f)()) {
	if (units == 0)
		time_units = 1;
	else
		time_units = units;
	func = f;
#if defined (__AVR_ATmega1284P__) || (__AVR_AT90USB1287__) || (__AVR_ATmega1281__) || defined (__AVR_ATmega128RFA1__)
	cli();
	ASSR |= (1 << AS2);
	TCCR2A =(1<<WGM21);

	OCR2A = 32768/32/resolution - 1;
	TCCR2B =3;//prescale=32

	TCNT2 = 0;
	while(ASSR & (1 << TCN2UB));

	SMCR |= (1 <<  SE);
	TIMSK2 &= ~_BV (OCIE2A);
	sei();
#else
#error Unsupported CPU type
#endif
}
void MxTimer2::start() {
	count = 0;
	overflowing = 0;
#if defined (__AVR_ATmega1284P__) || (__AVR_AT90USB1287__) || (__AVR_ATmega1281__) || defined (__AVR_ATmega128RFA1__)
	TCNT2 = 0;
	TIMSK2 = _BV (OCIE2A); //�����ж�
#endif
}
void MxTimer2::stop() {
#if defined (__AVR_ATmega1284P__) || (__AVR_AT90USB1287__) || (__AVR_ATmega1281__) || defined (__AVR_ATmega128RFA1__)
	TIMSK2 &= ~_BV (OCIE2A); //��ֹ�ж�
#endif
}
//�ж�ʵ�ʴ�����
void MxTimer2::_overflow() {
	count += 1;
	if (count >= time_units && !overflowing) {
		overflowing = 1;
		count = count - time_units;
		(*func)();
		overflowing = 0;
	}
}
ISR(TIMER2_COMPA_vect) {
	MxTimer2::_overflow();
}

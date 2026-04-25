#include "stdlib.h"

/*
val을 10으로 나눠가면서 reverse_buf에 역순으로(앞에서부터)채움.
return하기 전에 배열을 뒤집음

TODO: 음수 처리
*/

#define ITOA_BUF_MAX 13

void itoa(int val, char* buf) {
	
	int i = 0, length = 0;
	char reverse_buf[ITOA_BUF_MAX] = {0,};

	if (val == 0) {
		buf[0] = '0';
		buf[1] = 0;
		return;
	}

	if (val < 0) {
		return;
	}

	for (; val != 0; ++i) {
		reverse_buf[i] = (int)'0' + val % 10;
		val = val / 10;
	}

	i--;

	length = i;
	buf[length] = 0;

	for (; i != -1; --i) {
		buf[length - i] = reverse_buf[i];
	}

	return;

}

void itohex(int val, char* buf) {
	char buf_reverse[ITOA_BUF_MAX] = {0,};
	int idx = 0, length = 0;

	while (val > 0) {
	    buf_reverse[idx++] = val % 16;
	    val /= 16;
	}

	length = --idx;

	for (; idx >= 0; --idx) {
		char c = buf_reverse[idx];
		if (c < 10) {
			buf[length - idx] = '0' + c;
		} else {
			buf[length - idx] = 'A' + (c - 10);
		}
	}

	return;
}
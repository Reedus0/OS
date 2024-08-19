#pragma once

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	out8(0x3D4, 0x0A);
	out8(0x3D5, (in8(0x3D5) & 0xC0) | cursor_start);

	out8(0x3D4, 0x0B);
	out8(0x3D5, (in8(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor() {
	out8(0x3D4, 0x0A);
	out8(0x3D5, 0x20);
}

void update_cursor(uint8_t x, uint8_t y, uint8_t max_columns) {
	uint16_t pos = y * max_columns + x;

	out8(0x3D4, 0x0F);
	out8(0x3D5, (uint8_t)(pos & 0xFF));
	out8(0x3D4, 0x0E);
	out8(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
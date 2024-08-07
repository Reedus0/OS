#include "handlers.h"
#include "kernel/panic.h"

void __attribute__((__cdecl__)) irq_handler(struct irq_data irq_data) {
    g_intettrut_handlers[irq_data.interrupt_number](&irq_data);
}

static void set_interrupt_handler(size_t number, size_t handler) {
    g_intettrut_handlers[number] = handler;
}

void setup_irq_handlers() {
    set_interrupt_handler(0, irq_div_zero);
    set_interrupt_handler(1, irq_empty);
    set_interrupt_handler(2, irq_empty);
    set_interrupt_handler(3, irq_empty);
    set_interrupt_handler(4, irq_empty);
    set_interrupt_handler(5, irq_empty);
    set_interrupt_handler(6, irq_invalid_opcode);
    set_interrupt_handler(7, irq_empty);
    set_interrupt_handler(8, irq_double_fault);
    set_interrupt_handler(9, irq_empty);
    set_interrupt_handler(10, irq_empty);
    set_interrupt_handler(11, irq_empty);
    set_interrupt_handler(12, irq_empty);
    set_interrupt_handler(13, irq_empty);
    set_interrupt_handler(14, irq_empty);
    set_interrupt_handler(15, irq_empty);
    set_interrupt_handler(16, irq_empty);
    set_interrupt_handler(17, irq_empty);
    set_interrupt_handler(18, irq_empty);
    set_interrupt_handler(19, irq_empty);
    set_interrupt_handler(20, irq_empty);
    set_interrupt_handler(21, irq_empty);
    set_interrupt_handler(22, irq_empty);
    set_interrupt_handler(23, irq_empty);
    set_interrupt_handler(24, irq_empty);
    set_interrupt_handler(25, irq_empty);
    set_interrupt_handler(26, irq_empty);
    set_interrupt_handler(27, irq_empty);
    set_interrupt_handler(28, irq_empty);
    set_interrupt_handler(29, irq_empty);
    set_interrupt_handler(30, irq_empty);
    set_interrupt_handler(31, irq_empty);
    set_interrupt_handler(32, irq_empty);
    set_interrupt_handler(33, irq_empty);
    set_interrupt_handler(34, irq_empty);
    set_interrupt_handler(35, irq_empty);
    set_interrupt_handler(36, irq_empty);
    set_interrupt_handler(37, irq_empty);
    set_interrupt_handler(38, irq_empty);
    set_interrupt_handler(39, irq_empty);
    set_interrupt_handler(40, irq_empty);
    set_interrupt_handler(41, irq_empty);
    set_interrupt_handler(42, irq_empty);
    set_interrupt_handler(43, irq_empty);
    set_interrupt_handler(44, irq_empty);
    set_interrupt_handler(45, irq_empty);
    set_interrupt_handler(46, irq_empty);
    set_interrupt_handler(47, irq_empty);
    set_interrupt_handler(48, irq_empty);
    set_interrupt_handler(49, irq_empty);
    set_interrupt_handler(50, irq_empty);
    set_interrupt_handler(51, irq_empty);
    set_interrupt_handler(52, irq_empty);
    set_interrupt_handler(53, irq_empty);
    set_interrupt_handler(54, irq_empty);
    set_interrupt_handler(55, irq_empty);
    set_interrupt_handler(56, irq_empty);
    set_interrupt_handler(57, irq_empty);
    set_interrupt_handler(58, irq_empty);
    set_interrupt_handler(59, irq_empty);
    set_interrupt_handler(60, irq_empty);
    set_interrupt_handler(61, irq_empty);
    set_interrupt_handler(62, irq_empty);
    set_interrupt_handler(63, irq_empty);
    set_interrupt_handler(64, irq_empty);
    set_interrupt_handler(65, irq_empty);
    set_interrupt_handler(66, irq_empty);
    set_interrupt_handler(67, irq_empty);
    set_interrupt_handler(68, irq_empty);
    set_interrupt_handler(69, irq_empty);
    set_interrupt_handler(70, irq_empty);
    set_interrupt_handler(71, irq_empty);
    set_interrupt_handler(72, irq_empty);
    set_interrupt_handler(73, irq_empty);
    set_interrupt_handler(74, irq_empty);
    set_interrupt_handler(75, irq_empty);
    set_interrupt_handler(76, irq_empty);
    set_interrupt_handler(77, irq_empty);
    set_interrupt_handler(78, irq_empty);
    set_interrupt_handler(79, irq_empty);
    set_interrupt_handler(80, irq_empty);
    set_interrupt_handler(81, irq_empty);
    set_interrupt_handler(82, irq_empty);
    set_interrupt_handler(83, irq_empty);
    set_interrupt_handler(84, irq_empty);
    set_interrupt_handler(85, irq_empty);
    set_interrupt_handler(86, irq_empty);
    set_interrupt_handler(87, irq_empty);
    set_interrupt_handler(88, irq_empty);
    set_interrupt_handler(89, irq_empty);
    set_interrupt_handler(90, irq_empty);
    set_interrupt_handler(91, irq_empty);
    set_interrupt_handler(92, irq_empty);
    set_interrupt_handler(93, irq_empty);
    set_interrupt_handler(94, irq_empty);
    set_interrupt_handler(95, irq_empty);
    set_interrupt_handler(96, irq_empty);
    set_interrupt_handler(97, irq_empty);
    set_interrupt_handler(98, irq_empty);
    set_interrupt_handler(99, irq_empty);
    set_interrupt_handler(100, irq_empty);
    set_interrupt_handler(101, irq_empty);
    set_interrupt_handler(102, irq_empty);
    set_interrupt_handler(103, irq_empty);
    set_interrupt_handler(104, irq_empty);
    set_interrupt_handler(105, irq_empty);
    set_interrupt_handler(106, irq_empty);
    set_interrupt_handler(107, irq_empty);
    set_interrupt_handler(108, irq_empty);
    set_interrupt_handler(109, irq_empty);
    set_interrupt_handler(110, irq_empty);
    set_interrupt_handler(111, irq_empty);
    set_interrupt_handler(112, irq_empty);
    set_interrupt_handler(113, irq_empty);
    set_interrupt_handler(114, irq_empty);
    set_interrupt_handler(115, irq_empty);
    set_interrupt_handler(116, irq_empty);
    set_interrupt_handler(117, irq_empty);
    set_interrupt_handler(118, irq_empty);
    set_interrupt_handler(119, irq_empty);
    set_interrupt_handler(120, irq_empty);
    set_interrupt_handler(121, irq_empty);
    set_interrupt_handler(122, irq_empty);
    set_interrupt_handler(123, irq_empty);
    set_interrupt_handler(124, irq_empty);
    set_interrupt_handler(125, irq_empty);
    set_interrupt_handler(126, irq_empty);
    set_interrupt_handler(127, irq_empty);
    set_interrupt_handler(128, irq_empty);
    set_interrupt_handler(129, irq_empty);
    set_interrupt_handler(130, irq_empty);
    set_interrupt_handler(131, irq_empty);
    set_interrupt_handler(132, irq_empty);
    set_interrupt_handler(133, irq_empty);
    set_interrupt_handler(134, irq_empty);
    set_interrupt_handler(135, irq_empty);
    set_interrupt_handler(136, irq_empty);
    set_interrupt_handler(137, irq_empty);
    set_interrupt_handler(138, irq_empty);
    set_interrupt_handler(139, irq_empty);
    set_interrupt_handler(140, irq_empty);
    set_interrupt_handler(141, irq_empty);
    set_interrupt_handler(142, irq_empty);
    set_interrupt_handler(143, irq_empty);
    set_interrupt_handler(144, irq_empty);
    set_interrupt_handler(145, irq_empty);
    set_interrupt_handler(146, irq_empty);
    set_interrupt_handler(147, irq_empty);
    set_interrupt_handler(148, irq_empty);
    set_interrupt_handler(149, irq_empty);
    set_interrupt_handler(150, irq_empty);
    set_interrupt_handler(151, irq_empty);
    set_interrupt_handler(152, irq_empty);
    set_interrupt_handler(153, irq_empty);
    set_interrupt_handler(154, irq_empty);
    set_interrupt_handler(155, irq_empty);
    set_interrupt_handler(156, irq_empty);
    set_interrupt_handler(157, irq_empty);
    set_interrupt_handler(158, irq_empty);
    set_interrupt_handler(159, irq_empty);
    set_interrupt_handler(160, irq_empty);
    set_interrupt_handler(161, irq_empty);
    set_interrupt_handler(162, irq_empty);
    set_interrupt_handler(163, irq_empty);
    set_interrupt_handler(164, irq_empty);
    set_interrupt_handler(165, irq_empty);
    set_interrupt_handler(166, irq_empty);
    set_interrupt_handler(167, irq_empty);
    set_interrupt_handler(168, irq_empty);
    set_interrupt_handler(169, irq_empty);
    set_interrupt_handler(170, irq_empty);
    set_interrupt_handler(171, irq_empty);
    set_interrupt_handler(172, irq_empty);
    set_interrupt_handler(173, irq_empty);
    set_interrupt_handler(174, irq_empty);
    set_interrupt_handler(175, irq_empty);
    set_interrupt_handler(176, irq_empty);
    set_interrupt_handler(177, irq_empty);
    set_interrupt_handler(178, irq_empty);
    set_interrupt_handler(179, irq_empty);
    set_interrupt_handler(180, irq_empty);
    set_interrupt_handler(181, irq_empty);
    set_interrupt_handler(182, irq_empty);
    set_interrupt_handler(183, irq_empty);
    set_interrupt_handler(184, irq_empty);
    set_interrupt_handler(185, irq_empty);
    set_interrupt_handler(186, irq_empty);
    set_interrupt_handler(187, irq_empty);
    set_interrupt_handler(188, irq_empty);
    set_interrupt_handler(189, irq_empty);
    set_interrupt_handler(190, irq_empty);
    set_interrupt_handler(191, irq_empty);
    set_interrupt_handler(192, irq_empty);
    set_interrupt_handler(193, irq_empty);
    set_interrupt_handler(194, irq_empty);
    set_interrupt_handler(195, irq_empty);
    set_interrupt_handler(196, irq_empty);
    set_interrupt_handler(197, irq_empty);
    set_interrupt_handler(198, irq_empty);
    set_interrupt_handler(199, irq_empty);
    set_interrupt_handler(200, irq_empty);
    set_interrupt_handler(201, irq_empty);
    set_interrupt_handler(202, irq_empty);
    set_interrupt_handler(203, irq_empty);
    set_interrupt_handler(204, irq_empty);
    set_interrupt_handler(205, irq_empty);
    set_interrupt_handler(206, irq_empty);
    set_interrupt_handler(207, irq_empty);
    set_interrupt_handler(208, irq_empty);
    set_interrupt_handler(209, irq_empty);
    set_interrupt_handler(210, irq_empty);
    set_interrupt_handler(211, irq_empty);
    set_interrupt_handler(212, irq_empty);
    set_interrupt_handler(213, irq_empty);
    set_interrupt_handler(214, irq_empty);
    set_interrupt_handler(215, irq_empty);
    set_interrupt_handler(216, irq_empty);
    set_interrupt_handler(217, irq_empty);
    set_interrupt_handler(218, irq_empty);
    set_interrupt_handler(219, irq_empty);
    set_interrupt_handler(220, irq_empty);
    set_interrupt_handler(221, irq_empty);
    set_interrupt_handler(222, irq_empty);
    set_interrupt_handler(223, irq_empty);
    set_interrupt_handler(224, irq_empty);
    set_interrupt_handler(225, irq_empty);
    set_interrupt_handler(226, irq_empty);
    set_interrupt_handler(227, irq_empty);
    set_interrupt_handler(228, irq_empty);
    set_interrupt_handler(229, irq_empty);
    set_interrupt_handler(230, irq_empty);
    set_interrupt_handler(231, irq_empty);
    set_interrupt_handler(232, irq_empty);
    set_interrupt_handler(233, irq_empty);
    set_interrupt_handler(234, irq_empty);
    set_interrupt_handler(235, irq_empty);
    set_interrupt_handler(236, irq_empty);
    set_interrupt_handler(237, irq_empty);
    set_interrupt_handler(238, irq_empty);
    set_interrupt_handler(239, irq_empty);
    set_interrupt_handler(240, irq_empty);
    set_interrupt_handler(241, irq_empty);
    set_interrupt_handler(242, irq_empty);
    set_interrupt_handler(243, irq_empty);
    set_interrupt_handler(244, irq_empty);
    set_interrupt_handler(245, irq_empty);
    set_interrupt_handler(246, irq_empty);
    set_interrupt_handler(247, irq_empty);
    set_interrupt_handler(248, irq_empty);
    set_interrupt_handler(249, irq_empty);
    set_interrupt_handler(250, irq_empty);
    set_interrupt_handler(251, irq_empty);
    set_interrupt_handler(252, irq_empty);
    set_interrupt_handler(253, irq_empty);
    set_interrupt_handler(254, irq_empty);
    set_interrupt_handler(255, irq_empty);
}

interrupt irq_empty(struct irq_data* irq_data) {

}

interrupt irq_div_zero(struct irq_data* irq_data) {
    print_string("Exception: Division error!\n");
    panic(irq_data->regs);
}

interrupt irq_double_fault(struct irq_data* irq_data) {
    print_string("Exception: Double fault!\n");
    panic(irq_data->regs);
}

interrupt irq_invalid_opcode(struct irq_data* irq_data) {
    print_string("Exception: Invalid opcode!\n");
    panic(irq_data->regs);
}
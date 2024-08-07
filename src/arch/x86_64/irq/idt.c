#include "idt.h"
#include "lidt.h"
#include "isr.h"

static void idt_descriptor_set_isr(struct idt_descriptor* idt_descriptor, size_t isr) {
    idt_descriptor->offset_1 = isr & 0xFFFF;
    idt_descriptor->offset_2 = (isr >> 16) & 0xFFFF;
    idt_descriptor->offset_3 = (isr >> 32) & 0xFFFFFFFF;
}

static void idt_descriptor_set_ist(struct idt_descriptor* idt_descriptor, uint8_t ist) {
    idt_descriptor->ist = ist;
}

static void idt_descriptor_set_selector(struct idt_descriptor* idt_descriptor, uint16_t selector) {
    idt_descriptor->selector = selector;
}

static void idt_descriptor_set_type(struct idt_descriptor* idt_descriptor, uint8_t* type) {
    idt_descriptor->type = type;
}

static void idt_descriptor_enable(struct idt_descriptor* idt_descriptor) {
    idt_descriptor->type |= 1 << 7;
}

static void idt_descriptor_disable(struct idt_descriptor* idt_descriptor) {
    idt_descriptor->type &= 0 << 7;
}

static void setup_isrs() {
    idt_descriptor_set_isr(&g_idt_descriptors[0], isr_0);
    idt_descriptor_set_isr(&g_idt_descriptors[1], isr_1);
    idt_descriptor_set_isr(&g_idt_descriptors[2], isr_2);
    idt_descriptor_set_isr(&g_idt_descriptors[3], isr_3);
    idt_descriptor_set_isr(&g_idt_descriptors[4], isr_4);
    idt_descriptor_set_isr(&g_idt_descriptors[5], isr_5);
    idt_descriptor_set_isr(&g_idt_descriptors[6], isr_6);
    idt_descriptor_set_isr(&g_idt_descriptors[7], isr_7);
    idt_descriptor_set_isr(&g_idt_descriptors[8], isr_8);
    idt_descriptor_set_isr(&g_idt_descriptors[9], isr_9);
    idt_descriptor_set_isr(&g_idt_descriptors[10], isr_10);
    idt_descriptor_set_isr(&g_idt_descriptors[11], isr_11);
    idt_descriptor_set_isr(&g_idt_descriptors[12], isr_12);
    idt_descriptor_set_isr(&g_idt_descriptors[13], isr_13);
    idt_descriptor_set_isr(&g_idt_descriptors[14], isr_14);
    idt_descriptor_set_isr(&g_idt_descriptors[15], isr_15);
    idt_descriptor_set_isr(&g_idt_descriptors[16], isr_16);
    idt_descriptor_set_isr(&g_idt_descriptors[17], isr_17);
    idt_descriptor_set_isr(&g_idt_descriptors[18], isr_18);
    idt_descriptor_set_isr(&g_idt_descriptors[19], isr_19);
    idt_descriptor_set_isr(&g_idt_descriptors[20], isr_20);
    idt_descriptor_set_isr(&g_idt_descriptors[21], isr_21);
    idt_descriptor_set_isr(&g_idt_descriptors[22], isr_22);
    idt_descriptor_set_isr(&g_idt_descriptors[23], isr_23);
    idt_descriptor_set_isr(&g_idt_descriptors[24], isr_24);
    idt_descriptor_set_isr(&g_idt_descriptors[25], isr_25);
    idt_descriptor_set_isr(&g_idt_descriptors[26], isr_26);
    idt_descriptor_set_isr(&g_idt_descriptors[27], isr_27);
    idt_descriptor_set_isr(&g_idt_descriptors[28], isr_28);
    idt_descriptor_set_isr(&g_idt_descriptors[29], isr_29);
    idt_descriptor_set_isr(&g_idt_descriptors[30], isr_30);
    idt_descriptor_set_isr(&g_idt_descriptors[31], isr_31);
    idt_descriptor_set_isr(&g_idt_descriptors[32], isr_32);
    idt_descriptor_set_isr(&g_idt_descriptors[33], isr_33);
    idt_descriptor_set_isr(&g_idt_descriptors[34], isr_34);
    idt_descriptor_set_isr(&g_idt_descriptors[35], isr_35);
    idt_descriptor_set_isr(&g_idt_descriptors[36], isr_36);
    idt_descriptor_set_isr(&g_idt_descriptors[37], isr_37);
    idt_descriptor_set_isr(&g_idt_descriptors[38], isr_38);
    idt_descriptor_set_isr(&g_idt_descriptors[39], isr_39);
    idt_descriptor_set_isr(&g_idt_descriptors[40], isr_40);
    idt_descriptor_set_isr(&g_idt_descriptors[41], isr_41);
    idt_descriptor_set_isr(&g_idt_descriptors[42], isr_42);
    idt_descriptor_set_isr(&g_idt_descriptors[43], isr_43);
    idt_descriptor_set_isr(&g_idt_descriptors[44], isr_44);
    idt_descriptor_set_isr(&g_idt_descriptors[45], isr_45);
    idt_descriptor_set_isr(&g_idt_descriptors[46], isr_46);
    idt_descriptor_set_isr(&g_idt_descriptors[47], isr_47);
    idt_descriptor_set_isr(&g_idt_descriptors[48], isr_48);
    idt_descriptor_set_isr(&g_idt_descriptors[49], isr_49);
    idt_descriptor_set_isr(&g_idt_descriptors[50], isr_50);
    idt_descriptor_set_isr(&g_idt_descriptors[51], isr_51);
    idt_descriptor_set_isr(&g_idt_descriptors[52], isr_52);
    idt_descriptor_set_isr(&g_idt_descriptors[53], isr_53);
    idt_descriptor_set_isr(&g_idt_descriptors[54], isr_54);
    idt_descriptor_set_isr(&g_idt_descriptors[55], isr_55);
    idt_descriptor_set_isr(&g_idt_descriptors[56], isr_56);
    idt_descriptor_set_isr(&g_idt_descriptors[57], isr_57);
    idt_descriptor_set_isr(&g_idt_descriptors[58], isr_58);
    idt_descriptor_set_isr(&g_idt_descriptors[59], isr_59);
    idt_descriptor_set_isr(&g_idt_descriptors[60], isr_60);
    idt_descriptor_set_isr(&g_idt_descriptors[61], isr_61);
    idt_descriptor_set_isr(&g_idt_descriptors[62], isr_62);
    idt_descriptor_set_isr(&g_idt_descriptors[63], isr_63);
    idt_descriptor_set_isr(&g_idt_descriptors[64], isr_64);
    idt_descriptor_set_isr(&g_idt_descriptors[65], isr_65);
    idt_descriptor_set_isr(&g_idt_descriptors[66], isr_66);
    idt_descriptor_set_isr(&g_idt_descriptors[67], isr_67);
    idt_descriptor_set_isr(&g_idt_descriptors[68], isr_68);
    idt_descriptor_set_isr(&g_idt_descriptors[69], isr_69);
    idt_descriptor_set_isr(&g_idt_descriptors[70], isr_70);
    idt_descriptor_set_isr(&g_idt_descriptors[71], isr_71);
    idt_descriptor_set_isr(&g_idt_descriptors[72], isr_72);
    idt_descriptor_set_isr(&g_idt_descriptors[73], isr_73);
    idt_descriptor_set_isr(&g_idt_descriptors[74], isr_74);
    idt_descriptor_set_isr(&g_idt_descriptors[75], isr_75);
    idt_descriptor_set_isr(&g_idt_descriptors[76], isr_76);
    idt_descriptor_set_isr(&g_idt_descriptors[77], isr_77);
    idt_descriptor_set_isr(&g_idt_descriptors[78], isr_78);
    idt_descriptor_set_isr(&g_idt_descriptors[79], isr_79);
    idt_descriptor_set_isr(&g_idt_descriptors[80], isr_80);
    idt_descriptor_set_isr(&g_idt_descriptors[81], isr_81);
    idt_descriptor_set_isr(&g_idt_descriptors[82], isr_82);
    idt_descriptor_set_isr(&g_idt_descriptors[83], isr_83);
    idt_descriptor_set_isr(&g_idt_descriptors[84], isr_84);
    idt_descriptor_set_isr(&g_idt_descriptors[85], isr_85);
    idt_descriptor_set_isr(&g_idt_descriptors[86], isr_86);
    idt_descriptor_set_isr(&g_idt_descriptors[87], isr_87);
    idt_descriptor_set_isr(&g_idt_descriptors[88], isr_88);
    idt_descriptor_set_isr(&g_idt_descriptors[89], isr_89);
    idt_descriptor_set_isr(&g_idt_descriptors[90], isr_90);
    idt_descriptor_set_isr(&g_idt_descriptors[91], isr_91);
    idt_descriptor_set_isr(&g_idt_descriptors[92], isr_92);
    idt_descriptor_set_isr(&g_idt_descriptors[93], isr_93);
    idt_descriptor_set_isr(&g_idt_descriptors[94], isr_94);
    idt_descriptor_set_isr(&g_idt_descriptors[95], isr_95);
    idt_descriptor_set_isr(&g_idt_descriptors[96], isr_96);
    idt_descriptor_set_isr(&g_idt_descriptors[97], isr_97);
    idt_descriptor_set_isr(&g_idt_descriptors[98], isr_98);
    idt_descriptor_set_isr(&g_idt_descriptors[99], isr_99);
    idt_descriptor_set_isr(&g_idt_descriptors[100], isr_100);
    idt_descriptor_set_isr(&g_idt_descriptors[101], isr_101);
    idt_descriptor_set_isr(&g_idt_descriptors[102], isr_102);
    idt_descriptor_set_isr(&g_idt_descriptors[103], isr_103);
    idt_descriptor_set_isr(&g_idt_descriptors[104], isr_104);
    idt_descriptor_set_isr(&g_idt_descriptors[105], isr_105);
    idt_descriptor_set_isr(&g_idt_descriptors[106], isr_106);
    idt_descriptor_set_isr(&g_idt_descriptors[107], isr_107);
    idt_descriptor_set_isr(&g_idt_descriptors[108], isr_108);
    idt_descriptor_set_isr(&g_idt_descriptors[109], isr_109);
    idt_descriptor_set_isr(&g_idt_descriptors[110], isr_110);
    idt_descriptor_set_isr(&g_idt_descriptors[111], isr_111);
    idt_descriptor_set_isr(&g_idt_descriptors[112], isr_112);
    idt_descriptor_set_isr(&g_idt_descriptors[113], isr_113);
    idt_descriptor_set_isr(&g_idt_descriptors[114], isr_114);
    idt_descriptor_set_isr(&g_idt_descriptors[115], isr_115);
    idt_descriptor_set_isr(&g_idt_descriptors[116], isr_116);
    idt_descriptor_set_isr(&g_idt_descriptors[117], isr_117);
    idt_descriptor_set_isr(&g_idt_descriptors[118], isr_118);
    idt_descriptor_set_isr(&g_idt_descriptors[119], isr_119);
    idt_descriptor_set_isr(&g_idt_descriptors[120], isr_120);
    idt_descriptor_set_isr(&g_idt_descriptors[121], isr_121);
    idt_descriptor_set_isr(&g_idt_descriptors[122], isr_122);
    idt_descriptor_set_isr(&g_idt_descriptors[123], isr_123);
    idt_descriptor_set_isr(&g_idt_descriptors[124], isr_124);
    idt_descriptor_set_isr(&g_idt_descriptors[125], isr_125);
    idt_descriptor_set_isr(&g_idt_descriptors[126], isr_126);
    idt_descriptor_set_isr(&g_idt_descriptors[127], isr_127);
    idt_descriptor_set_isr(&g_idt_descriptors[128], isr_128);
    idt_descriptor_set_isr(&g_idt_descriptors[129], isr_129);
    idt_descriptor_set_isr(&g_idt_descriptors[130], isr_130);
    idt_descriptor_set_isr(&g_idt_descriptors[131], isr_131);
    idt_descriptor_set_isr(&g_idt_descriptors[132], isr_132);
    idt_descriptor_set_isr(&g_idt_descriptors[133], isr_133);
    idt_descriptor_set_isr(&g_idt_descriptors[134], isr_134);
    idt_descriptor_set_isr(&g_idt_descriptors[135], isr_135);
    idt_descriptor_set_isr(&g_idt_descriptors[136], isr_136);
    idt_descriptor_set_isr(&g_idt_descriptors[137], isr_137);
    idt_descriptor_set_isr(&g_idt_descriptors[138], isr_138);
    idt_descriptor_set_isr(&g_idt_descriptors[139], isr_139);
    idt_descriptor_set_isr(&g_idt_descriptors[140], isr_140);
    idt_descriptor_set_isr(&g_idt_descriptors[141], isr_141);
    idt_descriptor_set_isr(&g_idt_descriptors[142], isr_142);
    idt_descriptor_set_isr(&g_idt_descriptors[143], isr_143);
    idt_descriptor_set_isr(&g_idt_descriptors[144], isr_144);
    idt_descriptor_set_isr(&g_idt_descriptors[145], isr_145);
    idt_descriptor_set_isr(&g_idt_descriptors[146], isr_146);
    idt_descriptor_set_isr(&g_idt_descriptors[147], isr_147);
    idt_descriptor_set_isr(&g_idt_descriptors[148], isr_148);
    idt_descriptor_set_isr(&g_idt_descriptors[149], isr_149);
    idt_descriptor_set_isr(&g_idt_descriptors[150], isr_150);
    idt_descriptor_set_isr(&g_idt_descriptors[151], isr_151);
    idt_descriptor_set_isr(&g_idt_descriptors[152], isr_152);
    idt_descriptor_set_isr(&g_idt_descriptors[153], isr_153);
    idt_descriptor_set_isr(&g_idt_descriptors[154], isr_154);
    idt_descriptor_set_isr(&g_idt_descriptors[155], isr_155);
    idt_descriptor_set_isr(&g_idt_descriptors[156], isr_156);
    idt_descriptor_set_isr(&g_idt_descriptors[157], isr_157);
    idt_descriptor_set_isr(&g_idt_descriptors[158], isr_158);
    idt_descriptor_set_isr(&g_idt_descriptors[159], isr_159);
    idt_descriptor_set_isr(&g_idt_descriptors[160], isr_160);
    idt_descriptor_set_isr(&g_idt_descriptors[161], isr_161);
    idt_descriptor_set_isr(&g_idt_descriptors[162], isr_162);
    idt_descriptor_set_isr(&g_idt_descriptors[163], isr_163);
    idt_descriptor_set_isr(&g_idt_descriptors[164], isr_164);
    idt_descriptor_set_isr(&g_idt_descriptors[165], isr_165);
    idt_descriptor_set_isr(&g_idt_descriptors[166], isr_166);
    idt_descriptor_set_isr(&g_idt_descriptors[167], isr_167);
    idt_descriptor_set_isr(&g_idt_descriptors[168], isr_168);
    idt_descriptor_set_isr(&g_idt_descriptors[169], isr_169);
    idt_descriptor_set_isr(&g_idt_descriptors[170], isr_170);
    idt_descriptor_set_isr(&g_idt_descriptors[171], isr_171);
    idt_descriptor_set_isr(&g_idt_descriptors[172], isr_172);
    idt_descriptor_set_isr(&g_idt_descriptors[173], isr_173);
    idt_descriptor_set_isr(&g_idt_descriptors[174], isr_174);
    idt_descriptor_set_isr(&g_idt_descriptors[175], isr_175);
    idt_descriptor_set_isr(&g_idt_descriptors[176], isr_176);
    idt_descriptor_set_isr(&g_idt_descriptors[177], isr_177);
    idt_descriptor_set_isr(&g_idt_descriptors[178], isr_178);
    idt_descriptor_set_isr(&g_idt_descriptors[179], isr_179);
    idt_descriptor_set_isr(&g_idt_descriptors[180], isr_180);
    idt_descriptor_set_isr(&g_idt_descriptors[181], isr_181);
    idt_descriptor_set_isr(&g_idt_descriptors[182], isr_182);
    idt_descriptor_set_isr(&g_idt_descriptors[183], isr_183);
    idt_descriptor_set_isr(&g_idt_descriptors[184], isr_184);
    idt_descriptor_set_isr(&g_idt_descriptors[185], isr_185);
    idt_descriptor_set_isr(&g_idt_descriptors[186], isr_186);
    idt_descriptor_set_isr(&g_idt_descriptors[187], isr_187);
    idt_descriptor_set_isr(&g_idt_descriptors[188], isr_188);
    idt_descriptor_set_isr(&g_idt_descriptors[189], isr_189);
    idt_descriptor_set_isr(&g_idt_descriptors[190], isr_190);
    idt_descriptor_set_isr(&g_idt_descriptors[191], isr_191);
    idt_descriptor_set_isr(&g_idt_descriptors[192], isr_192);
    idt_descriptor_set_isr(&g_idt_descriptors[193], isr_193);
    idt_descriptor_set_isr(&g_idt_descriptors[194], isr_194);
    idt_descriptor_set_isr(&g_idt_descriptors[195], isr_195);
    idt_descriptor_set_isr(&g_idt_descriptors[196], isr_196);
    idt_descriptor_set_isr(&g_idt_descriptors[197], isr_197);
    idt_descriptor_set_isr(&g_idt_descriptors[198], isr_198);
    idt_descriptor_set_isr(&g_idt_descriptors[199], isr_199);
    idt_descriptor_set_isr(&g_idt_descriptors[200], isr_200);
    idt_descriptor_set_isr(&g_idt_descriptors[201], isr_201);
    idt_descriptor_set_isr(&g_idt_descriptors[202], isr_202);
    idt_descriptor_set_isr(&g_idt_descriptors[203], isr_203);
    idt_descriptor_set_isr(&g_idt_descriptors[204], isr_204);
    idt_descriptor_set_isr(&g_idt_descriptors[205], isr_205);
    idt_descriptor_set_isr(&g_idt_descriptors[206], isr_206);
    idt_descriptor_set_isr(&g_idt_descriptors[207], isr_207);
    idt_descriptor_set_isr(&g_idt_descriptors[208], isr_208);
    idt_descriptor_set_isr(&g_idt_descriptors[209], isr_209);
    idt_descriptor_set_isr(&g_idt_descriptors[210], isr_210);
    idt_descriptor_set_isr(&g_idt_descriptors[211], isr_211);
    idt_descriptor_set_isr(&g_idt_descriptors[212], isr_212);
    idt_descriptor_set_isr(&g_idt_descriptors[213], isr_213);
    idt_descriptor_set_isr(&g_idt_descriptors[214], isr_214);
    idt_descriptor_set_isr(&g_idt_descriptors[215], isr_215);
    idt_descriptor_set_isr(&g_idt_descriptors[216], isr_216);
    idt_descriptor_set_isr(&g_idt_descriptors[217], isr_217);
    idt_descriptor_set_isr(&g_idt_descriptors[218], isr_218);
    idt_descriptor_set_isr(&g_idt_descriptors[219], isr_219);
    idt_descriptor_set_isr(&g_idt_descriptors[220], isr_220);
    idt_descriptor_set_isr(&g_idt_descriptors[221], isr_221);
    idt_descriptor_set_isr(&g_idt_descriptors[222], isr_222);
    idt_descriptor_set_isr(&g_idt_descriptors[223], isr_223);
    idt_descriptor_set_isr(&g_idt_descriptors[224], isr_224);
    idt_descriptor_set_isr(&g_idt_descriptors[225], isr_225);
    idt_descriptor_set_isr(&g_idt_descriptors[226], isr_226);
    idt_descriptor_set_isr(&g_idt_descriptors[227], isr_227);
    idt_descriptor_set_isr(&g_idt_descriptors[228], isr_228);
    idt_descriptor_set_isr(&g_idt_descriptors[229], isr_229);
    idt_descriptor_set_isr(&g_idt_descriptors[230], isr_230);
    idt_descriptor_set_isr(&g_idt_descriptors[231], isr_231);
    idt_descriptor_set_isr(&g_idt_descriptors[232], isr_232);
    idt_descriptor_set_isr(&g_idt_descriptors[233], isr_233);
    idt_descriptor_set_isr(&g_idt_descriptors[234], isr_234);
    idt_descriptor_set_isr(&g_idt_descriptors[235], isr_235);
    idt_descriptor_set_isr(&g_idt_descriptors[236], isr_236);
    idt_descriptor_set_isr(&g_idt_descriptors[237], isr_237);
    idt_descriptor_set_isr(&g_idt_descriptors[238], isr_238);
    idt_descriptor_set_isr(&g_idt_descriptors[239], isr_239);
    idt_descriptor_set_isr(&g_idt_descriptors[240], isr_240);
    idt_descriptor_set_isr(&g_idt_descriptors[241], isr_241);
    idt_descriptor_set_isr(&g_idt_descriptors[242], isr_242);
    idt_descriptor_set_isr(&g_idt_descriptors[243], isr_243);
    idt_descriptor_set_isr(&g_idt_descriptors[244], isr_244);
    idt_descriptor_set_isr(&g_idt_descriptors[245], isr_245);
    idt_descriptor_set_isr(&g_idt_descriptors[246], isr_246);
    idt_descriptor_set_isr(&g_idt_descriptors[247], isr_247);
    idt_descriptor_set_isr(&g_idt_descriptors[248], isr_248);
    idt_descriptor_set_isr(&g_idt_descriptors[249], isr_249);
    idt_descriptor_set_isr(&g_idt_descriptors[250], isr_250);
    idt_descriptor_set_isr(&g_idt_descriptors[251], isr_251);
    idt_descriptor_set_isr(&g_idt_descriptors[252], isr_252);
    idt_descriptor_set_isr(&g_idt_descriptors[253], isr_253);
    idt_descriptor_set_isr(&g_idt_descriptors[254], isr_254);
    idt_descriptor_set_isr(&g_idt_descriptors[255], isr_255);
}

void setup_idt() {
    for (size_t i = 0; i < IDT_DESCRIPTORS_COUNT; i++) {
        struct idt_descriptor* current_idt_descriptor = &g_idt_descriptors[i];
        idt_descriptor_set_ist(current_idt_descriptor, 0);
        idt_descriptor_set_selector(current_idt_descriptor, 0 | (0 << 2) | (1 << 3));
        idt_descriptor_set_type(current_idt_descriptor, 0xe | (0 << 4));
        idt_descriptor_enable(current_idt_descriptor);
    };

    setup_isrs();

    g_idt.size = IDT_DESCRIPTORS_COUNT * sizeof(struct idt_descriptor) - 1;
    g_idt.offset = &g_idt_descriptors;

    load_idt(&g_idt);
}
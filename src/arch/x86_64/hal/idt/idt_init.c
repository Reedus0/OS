#include "idt.h"
#include "idt_init.h"

void init_isrs() {
    set_isr(&g_idt_descriptors[0], isr_0);
    set_isr(&g_idt_descriptors[1], isr_1);
    set_isr(&g_idt_descriptors[2], isr_2);
    set_isr(&g_idt_descriptors[3], isr_3);
    set_isr(&g_idt_descriptors[4], isr_4);
    set_isr(&g_idt_descriptors[5], isr_5);
    set_isr(&g_idt_descriptors[6], isr_6);
    set_isr(&g_idt_descriptors[7], isr_7);
    set_isr(&g_idt_descriptors[8], isr_8);
    set_isr(&g_idt_descriptors[9], isr_9);
    set_isr(&g_idt_descriptors[10], isr_10);
    set_isr(&g_idt_descriptors[11], isr_11);
    set_isr(&g_idt_descriptors[12], isr_12);
    set_isr(&g_idt_descriptors[13], isr_13);
    set_isr(&g_idt_descriptors[14], isr_14);
    set_isr(&g_idt_descriptors[15], isr_15);
    set_isr(&g_idt_descriptors[16], isr_16);
    set_isr(&g_idt_descriptors[17], isr_17);
    set_isr(&g_idt_descriptors[18], isr_18);
    set_isr(&g_idt_descriptors[19], isr_19);
    set_isr(&g_idt_descriptors[20], isr_20);
    set_isr(&g_idt_descriptors[21], isr_21);
    set_isr(&g_idt_descriptors[22], isr_22);
    set_isr(&g_idt_descriptors[23], isr_23);
    set_isr(&g_idt_descriptors[24], isr_24);
    set_isr(&g_idt_descriptors[25], isr_25);
    set_isr(&g_idt_descriptors[26], isr_26);
    set_isr(&g_idt_descriptors[27], isr_27);
    set_isr(&g_idt_descriptors[28], isr_28);
    set_isr(&g_idt_descriptors[29], isr_29);
    set_isr(&g_idt_descriptors[30], isr_30);
    set_isr(&g_idt_descriptors[31], isr_31);
    set_isr(&g_idt_descriptors[32], isr_32);
    set_isr(&g_idt_descriptors[33], isr_33);
    set_isr(&g_idt_descriptors[34], isr_34);
    set_isr(&g_idt_descriptors[35], isr_35);
    set_isr(&g_idt_descriptors[36], isr_36);
    set_isr(&g_idt_descriptors[37], isr_37);
    set_isr(&g_idt_descriptors[38], isr_38);
    set_isr(&g_idt_descriptors[39], isr_39);
    set_isr(&g_idt_descriptors[40], isr_40);
    set_isr(&g_idt_descriptors[41], isr_41);
    set_isr(&g_idt_descriptors[42], isr_42);
    set_isr(&g_idt_descriptors[43], isr_43);
    set_isr(&g_idt_descriptors[44], isr_44);
    set_isr(&g_idt_descriptors[45], isr_45);
    set_isr(&g_idt_descriptors[46], isr_46);
    set_isr(&g_idt_descriptors[47], isr_47);
    set_isr(&g_idt_descriptors[48], isr_48);
    set_isr(&g_idt_descriptors[49], isr_49);
    set_isr(&g_idt_descriptors[50], isr_50);
    set_isr(&g_idt_descriptors[51], isr_51);
    set_isr(&g_idt_descriptors[52], isr_52);
    set_isr(&g_idt_descriptors[53], isr_53);
    set_isr(&g_idt_descriptors[54], isr_54);
    set_isr(&g_idt_descriptors[55], isr_55);
    set_isr(&g_idt_descriptors[56], isr_56);
    set_isr(&g_idt_descriptors[57], isr_57);
    set_isr(&g_idt_descriptors[58], isr_58);
    set_isr(&g_idt_descriptors[59], isr_59);
    set_isr(&g_idt_descriptors[60], isr_60);
    set_isr(&g_idt_descriptors[61], isr_61);
    set_isr(&g_idt_descriptors[62], isr_62);
    set_isr(&g_idt_descriptors[63], isr_63);
    set_isr(&g_idt_descriptors[64], isr_64);
    set_isr(&g_idt_descriptors[65], isr_65);
    set_isr(&g_idt_descriptors[66], isr_66);
    set_isr(&g_idt_descriptors[67], isr_67);
    set_isr(&g_idt_descriptors[68], isr_68);
    set_isr(&g_idt_descriptors[69], isr_69);
    set_isr(&g_idt_descriptors[70], isr_70);
    set_isr(&g_idt_descriptors[71], isr_71);
    set_isr(&g_idt_descriptors[72], isr_72);
    set_isr(&g_idt_descriptors[73], isr_73);
    set_isr(&g_idt_descriptors[74], isr_74);
    set_isr(&g_idt_descriptors[75], isr_75);
    set_isr(&g_idt_descriptors[76], isr_76);
    set_isr(&g_idt_descriptors[77], isr_77);
    set_isr(&g_idt_descriptors[78], isr_78);
    set_isr(&g_idt_descriptors[79], isr_79);
    set_isr(&g_idt_descriptors[80], isr_80);
    set_isr(&g_idt_descriptors[81], isr_81);
    set_isr(&g_idt_descriptors[82], isr_82);
    set_isr(&g_idt_descriptors[83], isr_83);
    set_isr(&g_idt_descriptors[84], isr_84);
    set_isr(&g_idt_descriptors[85], isr_85);
    set_isr(&g_idt_descriptors[86], isr_86);
    set_isr(&g_idt_descriptors[87], isr_87);
    set_isr(&g_idt_descriptors[88], isr_88);
    set_isr(&g_idt_descriptors[89], isr_89);
    set_isr(&g_idt_descriptors[90], isr_90);
    set_isr(&g_idt_descriptors[91], isr_91);
    set_isr(&g_idt_descriptors[92], isr_92);
    set_isr(&g_idt_descriptors[93], isr_93);
    set_isr(&g_idt_descriptors[94], isr_94);
    set_isr(&g_idt_descriptors[95], isr_95);
    set_isr(&g_idt_descriptors[96], isr_96);
    set_isr(&g_idt_descriptors[97], isr_97);
    set_isr(&g_idt_descriptors[98], isr_98);
    set_isr(&g_idt_descriptors[99], isr_99);
    set_isr(&g_idt_descriptors[100], isr_100);
    set_isr(&g_idt_descriptors[101], isr_101);
    set_isr(&g_idt_descriptors[102], isr_102);
    set_isr(&g_idt_descriptors[103], isr_103);
    set_isr(&g_idt_descriptors[104], isr_104);
    set_isr(&g_idt_descriptors[105], isr_105);
    set_isr(&g_idt_descriptors[106], isr_106);
    set_isr(&g_idt_descriptors[107], isr_107);
    set_isr(&g_idt_descriptors[108], isr_108);
    set_isr(&g_idt_descriptors[109], isr_109);
    set_isr(&g_idt_descriptors[110], isr_110);
    set_isr(&g_idt_descriptors[111], isr_111);
    set_isr(&g_idt_descriptors[112], isr_112);
    set_isr(&g_idt_descriptors[113], isr_113);
    set_isr(&g_idt_descriptors[114], isr_114);
    set_isr(&g_idt_descriptors[115], isr_115);
    set_isr(&g_idt_descriptors[116], isr_116);
    set_isr(&g_idt_descriptors[117], isr_117);
    set_isr(&g_idt_descriptors[118], isr_118);
    set_isr(&g_idt_descriptors[119], isr_119);
    set_isr(&g_idt_descriptors[120], isr_120);
    set_isr(&g_idt_descriptors[121], isr_121);
    set_isr(&g_idt_descriptors[122], isr_122);
    set_isr(&g_idt_descriptors[123], isr_123);
    set_isr(&g_idt_descriptors[124], isr_124);
    set_isr(&g_idt_descriptors[125], isr_125);
    set_isr(&g_idt_descriptors[126], isr_126);
    set_isr(&g_idt_descriptors[127], isr_127);
    set_isr(&g_idt_descriptors[128], isr_128);
    set_isr(&g_idt_descriptors[129], isr_129);
    set_isr(&g_idt_descriptors[130], isr_130);
    set_isr(&g_idt_descriptors[131], isr_131);
    set_isr(&g_idt_descriptors[132], isr_132);
    set_isr(&g_idt_descriptors[133], isr_133);
    set_isr(&g_idt_descriptors[134], isr_134);
    set_isr(&g_idt_descriptors[135], isr_135);
    set_isr(&g_idt_descriptors[136], isr_136);
    set_isr(&g_idt_descriptors[137], isr_137);
    set_isr(&g_idt_descriptors[138], isr_138);
    set_isr(&g_idt_descriptors[139], isr_139);
    set_isr(&g_idt_descriptors[140], isr_140);
    set_isr(&g_idt_descriptors[141], isr_141);
    set_isr(&g_idt_descriptors[142], isr_142);
    set_isr(&g_idt_descriptors[143], isr_143);
    set_isr(&g_idt_descriptors[144], isr_144);
    set_isr(&g_idt_descriptors[145], isr_145);
    set_isr(&g_idt_descriptors[146], isr_146);
    set_isr(&g_idt_descriptors[147], isr_147);
    set_isr(&g_idt_descriptors[148], isr_148);
    set_isr(&g_idt_descriptors[149], isr_149);
    set_isr(&g_idt_descriptors[150], isr_150);
    set_isr(&g_idt_descriptors[151], isr_151);
    set_isr(&g_idt_descriptors[152], isr_152);
    set_isr(&g_idt_descriptors[153], isr_153);
    set_isr(&g_idt_descriptors[154], isr_154);
    set_isr(&g_idt_descriptors[155], isr_155);
    set_isr(&g_idt_descriptors[156], isr_156);
    set_isr(&g_idt_descriptors[157], isr_157);
    set_isr(&g_idt_descriptors[158], isr_158);
    set_isr(&g_idt_descriptors[159], isr_159);
    set_isr(&g_idt_descriptors[160], isr_160);
    set_isr(&g_idt_descriptors[161], isr_161);
    set_isr(&g_idt_descriptors[162], isr_162);
    set_isr(&g_idt_descriptors[163], isr_163);
    set_isr(&g_idt_descriptors[164], isr_164);
    set_isr(&g_idt_descriptors[165], isr_165);
    set_isr(&g_idt_descriptors[166], isr_166);
    set_isr(&g_idt_descriptors[167], isr_167);
    set_isr(&g_idt_descriptors[168], isr_168);
    set_isr(&g_idt_descriptors[169], isr_169);
    set_isr(&g_idt_descriptors[170], isr_170);
    set_isr(&g_idt_descriptors[171], isr_171);
    set_isr(&g_idt_descriptors[172], isr_172);
    set_isr(&g_idt_descriptors[173], isr_173);
    set_isr(&g_idt_descriptors[174], isr_174);
    set_isr(&g_idt_descriptors[175], isr_175);
    set_isr(&g_idt_descriptors[176], isr_176);
    set_isr(&g_idt_descriptors[177], isr_177);
    set_isr(&g_idt_descriptors[178], isr_178);
    set_isr(&g_idt_descriptors[179], isr_179);
    set_isr(&g_idt_descriptors[180], isr_180);
    set_isr(&g_idt_descriptors[181], isr_181);
    set_isr(&g_idt_descriptors[182], isr_182);
    set_isr(&g_idt_descriptors[183], isr_183);
    set_isr(&g_idt_descriptors[184], isr_184);
    set_isr(&g_idt_descriptors[185], isr_185);
    set_isr(&g_idt_descriptors[186], isr_186);
    set_isr(&g_idt_descriptors[187], isr_187);
    set_isr(&g_idt_descriptors[188], isr_188);
    set_isr(&g_idt_descriptors[189], isr_189);
    set_isr(&g_idt_descriptors[190], isr_190);
    set_isr(&g_idt_descriptors[191], isr_191);
    set_isr(&g_idt_descriptors[192], isr_192);
    set_isr(&g_idt_descriptors[193], isr_193);
    set_isr(&g_idt_descriptors[194], isr_194);
    set_isr(&g_idt_descriptors[195], isr_195);
    set_isr(&g_idt_descriptors[196], isr_196);
    set_isr(&g_idt_descriptors[197], isr_197);
    set_isr(&g_idt_descriptors[198], isr_198);
    set_isr(&g_idt_descriptors[199], isr_199);
    set_isr(&g_idt_descriptors[200], isr_200);
    set_isr(&g_idt_descriptors[201], isr_201);
    set_isr(&g_idt_descriptors[202], isr_202);
    set_isr(&g_idt_descriptors[203], isr_203);
    set_isr(&g_idt_descriptors[204], isr_204);
    set_isr(&g_idt_descriptors[205], isr_205);
    set_isr(&g_idt_descriptors[206], isr_206);
    set_isr(&g_idt_descriptors[207], isr_207);
    set_isr(&g_idt_descriptors[208], isr_208);
    set_isr(&g_idt_descriptors[209], isr_209);
    set_isr(&g_idt_descriptors[210], isr_210);
    set_isr(&g_idt_descriptors[211], isr_211);
    set_isr(&g_idt_descriptors[212], isr_212);
    set_isr(&g_idt_descriptors[213], isr_213);
    set_isr(&g_idt_descriptors[214], isr_214);
    set_isr(&g_idt_descriptors[215], isr_215);
    set_isr(&g_idt_descriptors[216], isr_216);
    set_isr(&g_idt_descriptors[217], isr_217);
    set_isr(&g_idt_descriptors[218], isr_218);
    set_isr(&g_idt_descriptors[219], isr_219);
    set_isr(&g_idt_descriptors[220], isr_220);
    set_isr(&g_idt_descriptors[221], isr_221);
    set_isr(&g_idt_descriptors[222], isr_222);
    set_isr(&g_idt_descriptors[223], isr_223);
    set_isr(&g_idt_descriptors[224], isr_224);
    set_isr(&g_idt_descriptors[225], isr_225);
    set_isr(&g_idt_descriptors[226], isr_226);
    set_isr(&g_idt_descriptors[227], isr_227);
    set_isr(&g_idt_descriptors[228], isr_228);
    set_isr(&g_idt_descriptors[229], isr_229);
    set_isr(&g_idt_descriptors[230], isr_230);
    set_isr(&g_idt_descriptors[231], isr_231);
    set_isr(&g_idt_descriptors[232], isr_232);
    set_isr(&g_idt_descriptors[233], isr_233);
    set_isr(&g_idt_descriptors[234], isr_234);
    set_isr(&g_idt_descriptors[235], isr_235);
    set_isr(&g_idt_descriptors[236], isr_236);
    set_isr(&g_idt_descriptors[237], isr_237);
    set_isr(&g_idt_descriptors[238], isr_238);
    set_isr(&g_idt_descriptors[239], isr_239);
    set_isr(&g_idt_descriptors[240], isr_240);
    set_isr(&g_idt_descriptors[241], isr_241);
    set_isr(&g_idt_descriptors[242], isr_242);
    set_isr(&g_idt_descriptors[243], isr_243);
    set_isr(&g_idt_descriptors[244], isr_244);
    set_isr(&g_idt_descriptors[245], isr_245);
    set_isr(&g_idt_descriptors[246], isr_246);
    set_isr(&g_idt_descriptors[247], isr_247);
    set_isr(&g_idt_descriptors[248], isr_248);
    set_isr(&g_idt_descriptors[249], isr_249);
    set_isr(&g_idt_descriptors[250], isr_250);
    set_isr(&g_idt_descriptors[251], isr_251);
    set_isr(&g_idt_descriptors[252], isr_252);
    set_isr(&g_idt_descriptors[253], isr_253);
    set_isr(&g_idt_descriptors[254], isr_254);
    set_isr(&g_idt_descriptors[255], isr_255);
}
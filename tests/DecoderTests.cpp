// ---------------------------------------------------------------------------
// Brizo tests                                                        by DEATH
// ---------------------------------------------------------------------------
// $Workfile: DecoderTests.cpp $ $Author: Death $
// $Revision: 37 $ $Date: 9/15/05 2:05 $
// ---------------------------------------------------------------------------
#include "DecoderTests.h"

#include <cassert>

#include "brizo/brizo.h"
#include "brizo/opcode.h"
#include "brizo/operand_values.h"
#include "brizo/types.h"
#include "brizo/decoder.h"

#include "brizo/exhaustion/pad_with_zeros.h"

#include "Helper.h"

using namespace brizo;

CPPUNIT_TEST_SUITE_REGISTRATION(DecoderTests);

void DecoderTests::setUp()
{
}

void DecoderTests::tearDown()
{
}

void DecoderTests::testNoData()
{
    const byte_type x = 0x00;
    const instruction inst = GetInstruction(&x, 0);
}

void DecoderTests::testOneByteOpcode()
{
    CPPUNIT_ASSERT(opcode::add == Decode(0x00, 0x00).get_opcode());
}

void DecoderTests::testUnknownOpcode()
{
    CPPUNIT_ASSERT(opcode::unknown == Decode(0xD6).get_opcode());
}

void DecoderTests::testPrefixedOneByteOpcode()
{
    const instruction & inst = Decode(0x66, 0x00, 0x00);
    CPPUNIT_ASSERT(opcode::add == inst.get_opcode());
    CPPUNIT_ASSERT(0x66 == inst.get_prefixes().front());
}

void DecoderTests::testPrefixedUnknownOpcode()
{
    const instruction & inst = Decode(0x66, 0xD6);
    CPPUNIT_ASSERT(opcode::unknown == inst.get_opcode());
    CPPUNIT_ASSERT(0x66 == inst.get_prefixes().front());
}

void DecoderTests::testJustPrefix()
{
    Decode(0x66);
}

void DecoderTests::testNopOpcode()
{
    CPPUNIT_ASSERT(opcode::nop == Decode(0x90).get_opcode());
}

void DecoderTests::testPauseOpcode()
{
    CPPUNIT_ASSERT(opcode::pause == Decode(0xF3, 0x90).get_opcode());
}

void DecoderTests::testPrefixedNopOpcode()
{
    const instruction & inst = Decode(0x66, 0x90);
    CPPUNIT_ASSERT(opcode::nop == inst.get_opcode());
    CPPUNIT_ASSERT(0x66 == inst.get_prefixes().front());
}

void DecoderTests::testMRMBothOneByteOpcode()
{
    const instruction & inst = Decode(0x80, 0x12, 0x00);
    CPPUNIT_ASSERT(opcode::adc == inst.get_opcode());
    CPPUNIT_ASSERT(0x12 == inst.get_modrm());
}

void DecoderTests::testFPUMRMLessEqBFOpcode()
{
    const instruction & inst = Decode(0xD8, 0x12);
    CPPUNIT_ASSERT(opcode::fcom == inst.get_opcode());
    CPPUNIT_ASSERT(0x12 == inst.get_modrm());
}

void DecoderTests::testFPUMRMGreaterBFOpcode()
{
    const instruction & inst = Decode(0xD9, 0xD0);
    CPPUNIT_ASSERT(opcode::fnop == inst.get_opcode());
    CPPUNIT_ASSERT(0xD0 == inst.get_modrm());
}

void DecoderTests::testTwoByteOpcode()
{
    CPPUNIT_ASSERT(opcode::lar == Decode(0x0F, 0x02, 0x00).get_opcode());
}

void DecoderTests::testMRMBothTwoByteOpcode()
{
    const instruction & inst = Decode(0x0F, 0x00, 0x42);
    CPPUNIT_ASSERT(opcode::sldt == inst.get_opcode());
    CPPUNIT_ASSERT(0x42 == inst.get_modrm());
}

void DecoderTests::testMRMFullMemTwoByteOpcode()
{
    const instruction & inst = Decode(0x0F, 0x01, 0x42);
    CPPUNIT_ASSERT(opcode::sgdt == inst.get_opcode());
    CPPUNIT_ASSERT(0x42 == inst.get_modrm());
}

void DecoderTests::testSSE3SpecialCase()
{
    const instruction & inst = Decode(0x0F, 0x01, 0xC9);
    CPPUNIT_ASSERT(opcode::mwait == inst.get_opcode());
    CPPUNIT_ASSERT(0xC9 == inst.get_modrm());
}

void DecoderTests::testPrefixedMRMLessEqBFTwoBytesOpcode()
{
    const instruction & inst = Decode(0x0F, 0x12, 0x42);
    CPPUNIT_ASSERT(opcode::movlps == inst.get_opcode());
    CPPUNIT_ASSERT(0x42 == inst.get_modrm());
}

void DecoderTests::testPrefixedMRMGreaterBFTwoBytesOpcode()
{
    const instruction & inst = Decode(0x0F, 0x12, 0xC9);
    CPPUNIT_ASSERT(opcode::movhlps == inst.get_opcode());
    CPPUNIT_ASSERT(0xC9 == inst.get_modrm());
}

void DecoderTests::testMRMMemTwoBytesOpcode()
{
    const instruction & inst = Decode(0x0F, 0x18, 0x12);
    CPPUNIT_ASSERT(opcode::prefetcht1 == inst.get_opcode());
    CPPUNIT_ASSERT(0x12 == inst.get_modrm());
}

void DecoderTests::testMRMMemTwoBytesOpcodeInvalid()
{
    CPPUNIT_ASSERT(opcode::unknown == Decode(0x0F, 0x18, 0xD2).get_opcode());
}

void DecoderTests::testMRM11OpcodeInvalid()
{
    CPPUNIT_ASSERT(opcode::unknown == Decode(0x0F, 0x71, 0x00).get_opcode());
}

void DecoderTests::testMRM11Opcode()
{
    CPPUNIT_ASSERT(opcode::psrlw == Decode(0x0F, 0x71, 0xD0, 0x00).get_opcode());
}

void DecoderTests::testBytes()
{
    const instruction & inst = Decode(0x90, 0x90);
    const instruction::bytes_type & bytes = inst.get_bytes();
    CPPUNIT_ASSERT(bytes.size() == 1);
    CPPUNIT_ASSERT(bytes.front() == 0x90);
}

void DecoderTests::testOperandValuesCount()
{
    const instruction & inst = Decode(0xF7, 0xD0);
    CPPUNIT_ASSERT(opcode::not_ == inst.get_opcode());
    CPPUNIT_ASSERT(inst.get_operand_values().size() == 1);
}

void DecoderTests::testRegisterValueWordDword()
{
    CheckOperandRegisterValue<0>(Decode(0xF7, 0xD0), reg::e_ax); // Ev
    CheckOperandRegisterValue<0>(Decode(0xF7, 0xD1), reg::e_cx); // Ev
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x02, 0x10), reg::e_dx); // Gv
}

void DecoderTests::testRegisterValueAbsDword()
{
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x7E, 0xF6), reg::esi); // Ed
    CheckOperandRegisterValue<0>(Decode(0xF2, 0x0F, 0x2C, 0x23), reg::esp); // Gd
}

void DecoderTests::testRegisterValueAbsWord()
{
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x00, 0xC6), reg::si); // Ew
    CheckOperandRegisterValue<1>(Decode(0x63, 0xC1), reg::ax); // Gw
}

void DecoderTests::testRegisterValueAbsByte()
{
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x90, 0xC1), reg::cl); // Eb
    CheckOperandRegisterValue<1>(Decode(0x0F, 0xB0, 0xDC), reg::bl); // Gb
}

void DecoderTests::testRegisterValueFixed()
{
    CheckOperandRegisterValue<0>(Decode(0xD8, 0xC0), reg::st0);
    CheckOperandRegisterValue<1>(Decode(0xD0, 0xC0), reg::val_1);
}

void DecoderTests::testModRmSegmentRegisterValue()
{
    CheckOperandRegisterValue<0>(Decode(0x8E, 0x22), reg::fs);
}

void DecoderTests::testModRmGeneralRegisterValue()
{
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x20, 0x67), reg::edi);
}

void DecoderTests::testControlRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0x20, 0x67), reg::cr4);
}

void DecoderTests::testDebugRegisterValue()
{
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x23, 0xDB), reg::dr3);
}

void DecoderTests::testTestRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0x24, 0xE8), reg::tr5);
}

void DecoderTests::testMMXRegisterValue()
{
    CheckOperandRegisterValue<0>(Decode(0x0F, 0x60, 0xE6), reg::mm4);
}

void DecoderTests::testXMMRegisterValue()
{
    CheckOperandRegisterValue<0>(Decode(0x66, 0x0F, 0x6E, 0xC0), reg::xmm0);
}

void DecoderTests::testMMXDRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0x60, 0xE6), reg::mm6);
}

void DecoderTests::testMMXQRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0x2A, 0xF3), reg::mm3);
}

void DecoderTests::testMMXDQRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x66, 0x0F, 0x2A, 0xF3), reg::mm3);
}

void DecoderTests::testMMXPIRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0xF7, 0xE1), reg::mm1);
}

void DecoderTests::testXMMSSRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0xF3, 0x0F, 0x10, 0xD3), reg::xmm3);
}

void DecoderTests::testXMMSDRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0xF2, 0x0F, 0x10, 0xD3), reg::xmm3);
}

void DecoderTests::testXMMQRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0x14, 0xFD), reg::xmm5);
}

void DecoderTests::testXMMPDRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x66, 0x0F, 0x10, 0xFD), reg::xmm5);
}

void DecoderTests::testXMMPSRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0x0F, 0x10, 0xFD), reg::xmm5);
}

void DecoderTests::testXMMDQRegisterValue()
{
    CheckOperandRegisterValue<1>(Decode(0xF3, 0x0F, 0x12, 0xFD), reg::xmm5);
}

void DecoderTests::testDirectAddress48()
{
    instruction inst = Decode(0xEA, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66);
    instruction::operand_value_type v = inst.get_operand_values().at(0);
    operand::direct_address_48_v & a =
        dynamic_cast<operand::direct_address_48_v &>(*v);
    CPPUNIT_ASSERT(a.get_segment() == 0x6655);
    CPPUNIT_ASSERT(a.get_address() == 0x44332211);
}

void DecoderTests::testDirectAddress32()
{
    instruction inst = Decode(0x66, 0xEA, 0x11, 0x22, 0x33, 0x44);
    instruction::operand_value_type v = inst.get_operand_values().at(0);
    operand::direct_address_32_v & a =
        dynamic_cast<operand::direct_address_32_v &>(*v);
    CPPUNIT_ASSERT(a.get_segment() == 0x4433);
    CPPUNIT_ASSERT(a.get_address() == 0x2211);
}

void DecoderTests::testPadWithZeros()
{
    typedef decoder<const byte_type *, pad_with_zeros> decoder;
    const byte_type data[] = {0x90};
    bool exhausted = false;
    decoder decode(data, data + 1, pad_with_zeros(exhausted));
    instruction one;
    const byte_type *first_iterator = decode(one);
    CPPUNIT_ASSERT_EQUAL(data + 1, first_iterator);
    CPPUNIT_ASSERT_EQUAL(false, exhausted);
    CPPUNIT_ASSERT(one.get_opcode() == opcode::nop);
    instruction two;
    const byte_type *second_iterator = decode(two);
    CPPUNIT_ASSERT_EQUAL(first_iterator, second_iterator);
    CPPUNIT_ASSERT_EQUAL(true, exhausted);
    CPPUNIT_ASSERT(two.get_opcode() == opcode::add);
}

void DecoderTests::testImmediate8()
{
    instruction inst = Decode(0xB0, 0x0B); // b00b!
    instruction::operand_value_type v = inst.get_operand_values().at(1);
    operand::immediate_8_v & i =
        dynamic_cast<operand::immediate_8_v &>(*v);
    CPPUNIT_ASSERT(i.get_value() == 0x0B);
}

void DecoderTests::testImmediate16()
{
    instruction inst = Decode(0xC2, 0xBE, 0xBA);
    instruction::operand_value_type v = inst.get_operand_values().at(0);
    operand::immediate_16_v & i =
        dynamic_cast<operand::immediate_16_v &>(*v);
    CPPUNIT_ASSERT(i.get_value() == 0xBABE);
}

void DecoderTests::testImmediate32()
{
    instruction inst = Decode(0x05, 0xEF, 0xBE, 0xAD, 0xDE);
    instruction::operand_value_type v = inst.get_operand_values().at(1);
    operand::immediate_32_v & i =
        dynamic_cast<operand::immediate_32_v &>(*v);
    CPPUNIT_ASSERT(i.get_value() == 0xDEADBEEF);
}

// ---------------------------------------------------------------------------
// Brizo tests                                                        by DEATH
// ---------------------------------------------------------------------------
// $Workfile: DecoderTests.h $ $Author: Death $
// $Revision: 24 $ $Date: 9/15/05 2:05 $
// ---------------------------------------------------------------------------
#ifndef BRIZO_TESTS_INC_DECODERTESTS_H
#define BRIZO_TESTS_INC_DECODERTESTS_H

#include <stdexcept>

#include <cppunit/extensions/HelperMacros.h>

class DecoderTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(DecoderTests);
        CPPUNIT_TEST_EXCEPTION(testNoData, std::runtime_error);
        CPPUNIT_TEST(testOneByteOpcode);
        CPPUNIT_TEST(testUnknownOpcode);
        CPPUNIT_TEST(testPrefixedOneByteOpcode);
        CPPUNIT_TEST(testPrefixedUnknownOpcode);
        CPPUNIT_TEST_EXCEPTION(testJustPrefix, std::runtime_error);
        CPPUNIT_TEST(testNopOpcode);
        CPPUNIT_TEST(testPauseOpcode);
        CPPUNIT_TEST(testPrefixedNopOpcode);
        CPPUNIT_TEST(testMRMBothOneByteOpcode);
        CPPUNIT_TEST(testFPUMRMLessEqBFOpcode);
        CPPUNIT_TEST(testFPUMRMGreaterBFOpcode);
        CPPUNIT_TEST(testTwoByteOpcode);
        CPPUNIT_TEST(testMRMBothTwoByteOpcode);
        CPPUNIT_TEST(testMRMFullMemTwoByteOpcode);
        CPPUNIT_TEST(testSSE3SpecialCase);
        CPPUNIT_TEST(testPrefixedMRMLessEqBFTwoBytesOpcode);
        CPPUNIT_TEST(testPrefixedMRMGreaterBFTwoBytesOpcode);
        CPPUNIT_TEST(testMRMMemTwoBytesOpcode);
        CPPUNIT_TEST(testMRMMemTwoBytesOpcodeInvalid);
        CPPUNIT_TEST(testMRM11OpcodeInvalid);
        CPPUNIT_TEST(testMRM11Opcode);
        CPPUNIT_TEST(testBytes);
        CPPUNIT_TEST(testOperandValuesCount);
        CPPUNIT_TEST(testRegisterValueWordDword);
        CPPUNIT_TEST(testRegisterValueAbsDword);
        CPPUNIT_TEST(testRegisterValueAbsWord);
        CPPUNIT_TEST(testRegisterValueAbsByte);
        CPPUNIT_TEST(testRegisterValueFixed);
        CPPUNIT_TEST(testModRmSegmentRegisterValue);
        CPPUNIT_TEST(testModRmGeneralRegisterValue);
        CPPUNIT_TEST(testControlRegisterValue);
        CPPUNIT_TEST(testDebugRegisterValue);
        CPPUNIT_TEST(testTestRegisterValue);
        CPPUNIT_TEST(testMMXRegisterValue);
        CPPUNIT_TEST(testXMMRegisterValue);
        CPPUNIT_TEST(testMMXDRegisterValue);
        CPPUNIT_TEST(testMMXQRegisterValue);
        CPPUNIT_TEST(testMMXDQRegisterValue);
        CPPUNIT_TEST(testMMXPIRegisterValue);
        CPPUNIT_TEST(testXMMSSRegisterValue);
        CPPUNIT_TEST(testXMMSDRegisterValue);
        CPPUNIT_TEST(testXMMQRegisterValue);
        CPPUNIT_TEST(testXMMPDRegisterValue);
        CPPUNIT_TEST(testXMMPSRegisterValue);
        CPPUNIT_TEST(testXMMDQRegisterValue);
        CPPUNIT_TEST(testDirectAddress48);
        CPPUNIT_TEST(testDirectAddress32);
        CPPUNIT_TEST(testPadWithZeros);
        CPPUNIT_TEST(testImmediate8);
        CPPUNIT_TEST(testImmediate16);
        CPPUNIT_TEST(testImmediate32);
    CPPUNIT_TEST_SUITE_END();

public:
    virtual void setUp();
    virtual void tearDown();

    void testNoData();
    void testOneByteOpcode();
    void testUnknownOpcode();
    void testPrefixedOneByteOpcode();
    void testPrefixedUnknownOpcode();
    void testJustPrefix();
    void testNopOpcode();
    void testPauseOpcode();
    void testPrefixedNopOpcode();
    void testMRMBothOneByteOpcode();
    void testFPUMRMLessEqBFOpcode();
    void testFPUMRMGreaterBFOpcode();
    void testTwoByteOpcode();
    void testMRMBothTwoByteOpcode();
    void testMRMFullMemTwoByteOpcode();
    void testSSE3SpecialCase();
    void testPrefixedMRMLessEqBFTwoBytesOpcode();
    void testPrefixedMRMGreaterBFTwoBytesOpcode();
    void testMRMMemTwoBytesOpcode();
    void testMRMMemTwoBytesOpcodeInvalid();
    void testMRM11OpcodeInvalid();
    void testMRM11Opcode();
    void testBytes();
    void testOperandValuesCount();
    void testRegisterValueWordDword();
    void testRegisterValueAbsDword();
    void testRegisterValueAbsWord();
    void testRegisterValueAbsByte();
    void testRegisterValueFixed();
    void testModRmSegmentRegisterValue();
    void testModRmGeneralRegisterValue();
    void testControlRegisterValue();
    void testDebugRegisterValue();
    void testTestRegisterValue();
    void testMMXRegisterValue();
    void testXMMRegisterValue();
    void testMMXDRegisterValue();
    void testMMXQRegisterValue();
    void testMMXDQRegisterValue();
    void testMMXPIRegisterValue();
    void testXMMSSRegisterValue();
    void testXMMSDRegisterValue();
    void testXMMQRegisterValue();
    void testXMMPDRegisterValue();
    void testXMMPSRegisterValue();
    void testXMMDQRegisterValue();
    void testDirectAddress48(); // only for 32-bit decoding, atm
    void testDirectAddress32(); // only for 32-bit decoding, atm
    void testPadWithZeros();
    void testImmediate8();
    void testImmediate16();
    void testImmediate32(); // only for 32-bit decoding, atm
};

#endif // BRIZO_TESTS_INC_DECODERTESTS_H

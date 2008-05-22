// ---------------------------------------------------------------------------
// Brizo tests                                                        by DEATH
// ---------------------------------------------------------------------------
// $Workfile: driver.cpp $ $Author: Death $
// $Revision: 3 $ $Date: 4/16/05 22:25 $
// ---------------------------------------------------------------------------
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main()
{
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(suite);
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));
    bool wasSuccessful = runner.run();
    return wasSuccessful ? 0 : 1;
}
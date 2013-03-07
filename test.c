#include "test.h"

#include <stdlib.h>
#include <string.h>

typedef struct Test_ResultType
{
	uint16_t totalTests;
	uint16_t successCount;
	uint16_t failureCount;
} Test_ResultType;

Test_TestHolder* m_Test_head;
Test_TestHolder* m_Test_activeTest;
Test_ResultType m_Test_result;

void Test_init(void)
{
	m_Test_head = NULL;
	m_Test_activeTest = NULL;
	
	m_Test_result.totalTests = 0;
	m_Test_result.successCount = 0;
	m_Test_result.failureCount = 0;
}

void Test_add(Test_TestHolder* test)
{
	// Put to front of chain
	test->next = m_Test_head;
	m_Test_head = test;
}

void Test_assertTrueLog(uint8_t condition, uint16_t lineNumber)
{
	// We have the active test
	if( !(condition) )
	{
		m_Test_activeTest->testResult = FAILURE;
		m_Test_activeTest->line = lineNumber;	
	}
}

void Test_assertEqualLog(uint16_t expected, uint16_t actual, 
    uint16_t lineNumber)
{
	if( expected != actual )
	{
		m_Test_activeTest->testResult = FAILURE;
		m_Test_activeTest->line = lineNumber;	
	}
}

// Run through all the tests
void Test_runall(void)
{
	// Reset counts
	m_Test_result.totalTests = 0;
	m_Test_result.successCount = 0;
	m_Test_result.failureCount = 0;
	
	// Reset status of all
	m_Test_activeTest = m_Test_head;
	while( m_Test_activeTest != NULL )
	{
		m_Test_result.totalTests++;
		
		m_Test_activeTest->testResult = NOT_RUN;
		m_Test_activeTest->line = 0;
		
		// next in the chain
		m_Test_activeTest = m_Test_activeTest->next;
	}
	
	// Now execute the tests
	m_Test_activeTest = m_Test_head;
	while( m_Test_activeTest != NULL )
	{
		m_Test_activeTest->testFunction();
		
		if( m_Test_activeTest->testResult == NOT_RUN )
		{
			m_Test_activeTest->testResult = SUCCESS;
			m_Test_result.successCount++;
		}
		else
		{
			m_Test_result.failureCount++;
		}
		
		asm("nop");
		
		// next in the chain
		m_Test_activeTest = m_Test_activeTest->next;
	}
	
	// Get the results
	asm("nop");
}

// Examples
/*
Test_test(Test, testWillFail)
{
	Test_assertEquals(1, 0);
}

Test_test(Test, testWillPass)
{
	Test_assertTrue(1);
}
*/

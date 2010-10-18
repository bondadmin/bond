#ifndef TEST_FRAMEWORK_TESTFRAMEWORK_H
#define TEST_FRAMEWORK_TESTFRAMEWORK_H

#define DEFINE_TEST(testName) bool __Test ## testName ## __(TestFramework::Logger &logger)

#define TEST_ITEM(testName) {#testName, &__Test ## testName ## __ },

#define RUN_TESTS(groupName, TEST_ITEMS)                               \
  int main()                                                           \
  {                                                                    \
    const TestFramework::TestItem groupName ## _Items[] =              \
    {                                                                  \
      TEST_ITEMS                                                       \
    };                                                                 \
                                                                       \
    const TestFramework::TestGroup groupName ## _Group =               \
    {                                                                  \
      #groupName,                                                      \
      groupName ## _Items,                                             \
      sizeof(groupName ## _Items) / sizeof(*groupName ## _Items)       \
    };                                                                 \
                                                                       \
    const bool success = TestFramework::RunTests(groupName ## _Group); \
    return success ? 0 : 1;                                            \
  }                                                                    \

#define ASSERT_MESSAGE(condition, message)                                  \
  __ASSERT_FORMAT__(condition, logger, __FILE__, __LINE__, ("%s", message)) \

#define ASSERT_FORMAT(condition, format)                                    \
  __ASSERT_FORMAT__(condition, logger, __FILE__, __LINE__, format)          \

#define __ASSERT_FORMAT__(condition, logger, file, line, format)            \
  if (!(condition))                                                         \
  {                                                                         \
    logger.Log("line %u in %s: ", line, file);                              \
    logger.Log format;                                                      \
    return false;                                                           \
  }                                                                         \

namespace TestFramework
{

class Logger
{
public:
	Logger();

	void Log(const char *format, ...);
	const char *GetLog() const { return mBuffer; }

private:
	static const unsigned BUFFER_SIZE = 1024;
	char mBuffer[BUFFER_SIZE];
	unsigned mIndex;
};

typedef bool TestFunction(Logger &logger);

struct TestItem
{
	const char *testName;
	const TestFunction *testFunction;
};

struct TestGroup
{
	const char *groupName;
	const TestItem *items;
	const unsigned numItems;
};

bool RunTests(const TestGroup &testGroup);

}

#endif
#include "gmock/gmock.h"
#include "device_driver.h"
#include "flash_memory_device.h"
#include "app.cpp"


/*
Device Driver, Read 기능 개발 명세서
▪ Read를5회수행한다.
▪ 5회의Read의결과가모두같은값일때, 읽은값을return 한다.
 • 모두같은값이아니라면, Custom Exception(ReadFailException)을 발생시킨다.


 Device Driver, Write 기능 개발 명세서
▪ 해당객체에이미값이적혀있는지확인한다.
 (읽었을때0xFF라면, 값이지워진상태이다.)
 ▪ 값이적혀있지않을때, 해당주소에값을Write한다.
 • 이미값이적혀있다면Custom Exception(WriteFailException)을 발생시킨다.
*/
using namespace testing;
class MockFlash : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long), (override));
	MOCK_METHOD(void, write, (long, unsigned char), (override));
};

class fixtureFlash : public testing::Test
{


protected :
	void SetUp(void) override
	{
		driver = new DeviceDriver(&mock);
	}
public:
	NiceMock<MockFlash> mock;
	DeviceDriver* driver;

};


TEST_F(fixtureFlash, ReadFromHW) {

	EXPECT_CALL(mock, read((long)0xB))
		.Times(5);

	int ret = driver->read(0xB);
}

TEST_F(fixtureFlash, WriteFromHW) {
	EXPECT_CALL(mock, read)
		.WillRepeatedly(testing::Return(0xFF));
	EXPECT_CALL(mock, write)
		.Times(1);

	driver->write(0, 0x8);

}


TEST_F(fixtureFlash, ReadFromHWAndException) {
	EXPECT_CALL(mock, read((long)0xB))
		.WillOnce(Return((unsigned char)0xA))
		.WillOnce(Return((unsigned char)0xA))
		.WillOnce(Return((unsigned char)0xB));
	EXPECT_THROW(driver->read(0xB), ReadFailException);
}

TEST_F(fixtureFlash, WriteFromHWAndException) {
	EXPECT_CALL(mock, read((long)0xB))
		.WillRepeatedly(Return((unsigned char)0xAA));

	EXPECT_THROW(driver->write(0xB, 0xBB), WriteFailException);
}

TEST_F(fixtureFlash, AppReadAndPrint) {
	EXPECT_CALL(mock, read)
		.WillRepeatedly(Return((unsigned char)0xAA));
	EXPECT_CALL(mock, read)
		.Times(100*5);

	Application app(driver);
	app.readAndPrint(0, 100);

}

TEST_F(fixtureFlash, AppWriteAll) {
	Application app(driver);
	EXPECT_CALL(mock, read)
		.WillRepeatedly(Return((unsigned char)0xFF));
	app.writeAll(0xEF);
}



int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
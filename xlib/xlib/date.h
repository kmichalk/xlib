//#ifndef _X_DATE_H_
//#define _X_DATE_H_
//
//#include <chrono>
//#include <iostream>
//
//
//namespace x
//{
//	class date
//	{
//	public:
//		static constexpr double UNIT_ = 
//			(double)std::chrono::system_clock::period::num /
//			std::chrono::system_clock::period::den;
//		//static constexpr long long unsigned NANO_SCALE_ = 1000;
//		//static constexpr long long unsigned MICRO_SCALE_ = NANO_SCALE_*1000;
//		static constexpr long long unsigned MILLI_SCALE_ = 10000;
//		static constexpr long long unsigned SECOND_SCALE_ = 60;
//		static constexpr long long unsigned MINUTE_SCALE_ = SECOND_SCALE_*60;
//		static constexpr long long unsigned HOUR_SCALE_ = MINUTE_SCALE_*24;
//		static constexpr long long unsigned DAY_COUNT_[][4] = {
//			{31,31,31,31},
//			{28,28,28,29},
//			{31,31,31,31},
//			{30,30,30,30},
//			{31,31,31,31},
//			{30,30,30,30},
//			{31,31,31,31},
//			{31,31,31,31},
//			{30,30,30,30},
//			{31,31,31,31},
//			{30,30,30,30},
//			{31,31,31,31}};
//		//static constexpr long long unsigned MONTH_SCALE_ = 1000;
//		//static constexpr long long unsigned YEAR_SCALE_ = 1000;
//
//		long long unsigned clock_;
//		unsigned short year_;
//		unsigned short month_;
//		unsigned short day_;
//		unsigned short hour_;
//		unsigned short minute_;
//		unsigned short second_;
//		unsigned short milli_;
//		unsigned short micro_;
//		unsigned short nano_;
//
//	public:
//		date();
//
//		friend std::ostream& operator<<(std::ostream& os, date const& d);
//
//	};
//
//}
//
//#endif //_X_DATE_H_

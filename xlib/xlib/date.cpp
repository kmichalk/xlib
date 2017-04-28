//#include "date.h"
//
//x::date::date():
//	clock_{(unsigned long long)std::chrono::system_clock{}.now().time_since_epoch().count()},
//	//nano_{unsigned short(clock_%NANO_SCALE_)},
//	//micro_{unsigned short(clock_%MICRO_SCALE_)},
//	//milli_{unsigned short(clock_%MILLI_SCALE_)},
//	second_{unsigned short(clock_%SECOND_SCALE_)},
//	minute_{unsigned short(clock_%MINUTE_SCALE_)},
//	hour_{unsigned short(clock_%HOUR_SCALE_)}
//{
//	long long unsigned yearDayScale =10000000* 60ull * 60ull * 24ull * 365ull;
//	year_ = clock_ / yearDayScale;
//	auto leap = year_ / 4;
//	auto leapCycle = year_ % 4;
//	year_ -= leap / 365;
//	auto yearDay = clock_/(yearDayScale/365)/365 - leap;
//	day_ = yearDay;
//	for (int m = 0; m<12; ++m) {
//		day_ -= DAY_COUNT_[m][leapCycle];
//		if (day_ < 1) {
//			day_ += DAY_COUNT_[m][leapCycle];
//			month_ = m + 1;
//			break;
//		}
//	}
//}
//
//std::ostream & x::operator<<(std::ostream & os, date const & d)
//{
//	return os << 
//		d.year_ << "." <<
//		d.month_ << "." <<
//		d.day_ << " " <<
//		d.hour_ << ":" <<
//		d.minute_ << ":" <<
//		d.second_;
//}

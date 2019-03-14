#ifndef SRC_TIME
#define SRC_TIME
/*  class Time
 *  用于表示时间与处理时间
*/
class Time
{
public:
  /* 接受一个形如dhh00的整数,将d作为日期,hh作为小时储存 
  */
  Time(int time);
  Time(int day, int hour);

  /* 获取当前时间与给定时间的差值,返回值为差值时间的拷贝(this - t)
  */
  Time time_diff(const Time &t) const;

  /* 将当前时间与给定时间相加,返回相加后时间的引用(改变了使用此方法的对象)
   */
  Time &add_time(const Time &t);
  Time &add_time(const int hour, const int day = 0);

  /* 获取当前时间与给定时间的差值,返回值为两者相差的小时数(this - t)
  */
  int hour_diff(const Time &) const;

  int GetDay() const { return day_; }
  int GetHour() const { return hour_; }

#ifdef TEST_TIME
#include <iostream>
  void print()
  {
    std::cout << "日期是: " << this->GetDay()
              << "\t时间是: " << this->GetHour() << std::endl;
  }
#endif // TEST_TIME

private:
  int day_ = 0; // day_为0表示当天
  int hour_ = 0;
  void set_time(Time &time, int day, int hour)
  {
    // todo: format check
    time.day_ = day;
    time.hour_ = hour;
  }
};

inline Time::Time(int time)
{
  int t_day = time / 10000, t_hour = (time - t_day * 10000) / 100;
  set_time(*this, t_day - 1, t_hour);
  
}

inline Time::Time(int day, int hour)
{
  set_time(*this, day - 1, hour);
}

inline Time Time::time_diff(const Time &t) const
{
  int temp_hour = hour_ - t.hour_, temp_day = day_ - t.day_;
  if (temp_hour < 0)
  {
    --temp_day;
    temp_hour += 24;
  }
  Time res(temp_day + 1, temp_hour);
  return res;
}

inline Time &Time::add_time(const Time &t)
{
  this->add_time(t.hour_, t.day_);
  return *this;
}

inline Time &Time::add_time(const int hour, const int day /* = 0 */)
{
  day_ += day;
  hour_ += hour;
  if (hour_ > 24)
  {
    hour_ -= 24;
    ++day_;
  }
  return *this;
}

inline int Time::hour_diff(const Time &t) const
{
  Time temp = this->time_diff(t);
  return temp.hour_ + temp.day_ * 24;
}

#endif // SRC_TIME
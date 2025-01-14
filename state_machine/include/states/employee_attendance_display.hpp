#pragma once
#include <shop.hpp>
#include <shop_state.hpp>

class EmployeeAttendanceDisplay : public ShopState {
 public:
  void enter(Shop* shop);
  void exit(Shop* shop);
  static ShopState& getInstance();

 private:
  EmployeeAttendanceDisplay();
  EmployeeAttendanceDisplay(const EmployeeAttendanceDisplay& other);
  EmployeeAttendanceDisplay& operator=(const EmployeeAttendanceDisplay& other);
};
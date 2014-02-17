/*
    Reporter is a simple class to track object lifetime and
    allocation/deallocation statistics. (c) Volodymyr Lashko 2014
*/

#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>


class Reporter
{
public:
  Reporter()
      : id_(nextId())
  {
    createRecord(id());
    std::cout << "id" << id() << " created" << std::endl;
  }
  
  ~Reporter()
  {
    removeRecord(id());
    std::cout << "id" << id() << " destroyed" << std::endl;
  }
  
  Reporter(const Reporter & other)
      : id_(nextId())
  {
    createRecord(id());
    std::cout << "id" << id() << " copied from id" << other.id() << std::endl;
  }
  
  static void report(bool detailed = true)
  {
    std::string livingList;
    if (detailed) {
      std::ostringstream oss;
      if (! allocatedObjects_.empty())
      {
        std::copy(allocatedObjects_.begin(),
                  allocatedObjects_.end(),
                  std::ostream_iterator< unsigned >(oss, " "));
      }
      livingList = "[ " + oss.str() + "]";
    }
    std::cout << (detailed ? "DETAILED STATISTICS" : "STATISTICS") << "\n"
              << "Created: " << nextId_ << " objects\n"
              << "Living: " << allocatedObjects_.size() << " objects " + livingList
              << std::endl;
  }

private:
  const unsigned id() const { return id_; }
  const unsigned id_;
  static unsigned nextId_;
  static const unsigned nextId() { return ++nextId_; }
  
  static std::vector< unsigned > allocatedObjects_;
  static void createRecord(const unsigned id) {
    allocatedObjects_.push_back(id);
  }
  static void removeRecord(const unsigned id) {
    allocatedObjects_.erase( std::remove( allocatedObjects_.begin(),
                                          allocatedObjects_.end(), id ),
                             allocatedObjects_.end() );
  }
};


unsigned Reporter::nextId_ = 0;
std::vector< unsigned > Reporter::allocatedObjects_;


#include <Windows.h>
#include <iostream>

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>

#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

int main(int argc, char** argv)
{
  // create color multi threaded logger
  auto console = spdlog::stdout_color_mt("injector");

  console->critical("doesn't actually do anything yet.\n");
  system("pause");

  return 0;
}
#pragma once

#include <application_api.hpp>

#ifdef __cplusplus
extern "C" {
#endif

OMNIFIC_APPLICATION_API void load_script_instances();
OMNIFIC_APPLICATION_API void on_input();
OMNIFIC_APPLICATION_API void on_start();
OMNIFIC_APPLICATION_API void on_early();
OMNIFIC_APPLICATION_API void on_logic();
OMNIFIC_APPLICATION_API void on_compute();
OMNIFIC_APPLICATION_API void on_late();
OMNIFIC_APPLICATION_API void on_finish();
OMNIFIC_APPLICATION_API void on_output();

#ifdef __cplusplus
}
#endif
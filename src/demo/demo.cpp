#include "demo.hpp"

void initialize_application(Omnific::Engine* omnific_engine)
{
  omnific_engine->add_app_data_directories(
    {
      PROJECT_SOURCE_DIRECTORY, 
      PROJECT_APP_DATA_DIRECTORY,
      PROJECT_BUILD_DIRECTORY
    }
	);
}
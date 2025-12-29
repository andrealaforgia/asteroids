#include "stage.h"

#include <stdlib.h>

void destroy_stage(stage_ptr stage) {
  if (stage != NULL) {
    if (stage->cleanup != NULL) {
      stage->cleanup(stage);
    }
    free(stage);
  }
}

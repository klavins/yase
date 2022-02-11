#ifndef YASE_SAMPLE_H
#define YASE_SAMPLE_H

#include "yase.hh"
#include "AudioFile.h"

namespace yase {

    class Sample : public Module {

    public:

      Sample(string path);
      void init();
      void update();
      void trigger();

    private:

      int left, right;
      int n, length;
      bool active;    
      AudioFile<double> audioFile;

    };

}

#endif

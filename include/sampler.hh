#ifndef YASE_SAMPLER_H
#define YASE_SAMPLER_H

#include "yase.hh"

namespace yase {

    class Sampler : public Module {

    public:

      const int BUFFER_LENGTH = SAMPLE_RATE; // one second of data

      Sampler(string path);

      void init();
      void update();

      Sampler &record();
      Sampler &stop();
      Sampler &write();

    private:

      vector<double *> buffers;
      string path;
      int index, signal;
      bool active;

    };

}

#endif

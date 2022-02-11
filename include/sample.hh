#ifndef YASE_SAMPLE_H
#define YASE_SAMPLE_H

#include "yase.hh"
#include "AudioFile.h"

namespace yase {

    //! A module that can be used to play a sample stored as a .wav file.
    //! For example, 
    //! \code{cpp}
    //! Sample sample("/path/to/a/wavefile.wav");
    //! Container container;
    //! Audio audio;
    //! 
    //! container.add(sample).add(audio);
    //! container.connect(sample, "left", audio, "left");
    //! container.connect(sample, "right", audio, "right");
    //!
    //! container.listen(123, [&] (const Event &e) {
    //!     sample.trigger();
    //! });;
    //! \endcode
    //!
    //! \param[out] left
    //! \param[out right]
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

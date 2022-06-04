// 
// YASE Sample Module Header
// 
// Copyright (C) 2022 Eric Klavins
// This file is part of YASE
// 
// YASE is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YASE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with YASE. If not, see <https://www.gnu.org/licenses/>.
// 

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
    //! \param[in] amplitude
    //! \param[out] left
    //! \param[out] right
    class Sample : public Module {

    public:

      Sample(string path);
      void init();
      void update();
      void trigger();
      void stop();
      inline int size() { return length; }

    private:

      int left, right, amplitude;
      int n, length;
      bool active;    
      AudioFile<double> audioFile;
      int num_channels;

    };

}

#endif

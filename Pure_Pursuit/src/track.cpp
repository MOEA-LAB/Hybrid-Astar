// Copyright 2019 Alexander Liniger

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#include "track.h"


TrackPos Track::getTrack()
{
    return {Eigen::Map<Eigen::VectorXd>(X.data(), X.size()), Eigen::Map<Eigen::VectorXd>(Y.data(), Y.size()),
            Eigen::Map<Eigen::VectorXd>(X_inner.data(), X_inner.size()), Eigen::Map<Eigen::VectorXd>(Y_inner.data(), Y_inner.size()),
            Eigen::Map<Eigen::VectorXd>(X_outer.data(), X_outer.size()), Eigen::Map<Eigen::VectorXd>(Y_outer.data(), Y_outer.size())};
}


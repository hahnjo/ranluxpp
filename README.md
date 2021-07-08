A Portable Implementation of RANLUX++
=====================================

Standalone version of the code originally submitted to the [ROOT data analysis framework](https://root.cern/)
and described in "A Portable Implementation of RANLUX++" at vCHEP2021 ([preprint on arXiv.org](https://arxiv.org/abs/2106.02504)).

Building
--------

The project uses the CMake build system:

```shell
 $ mkdir build && cd build
 $ cmake -DCMAKE_BUILD_TYPE=Release ..
 $ make
```

For more information about CMake, see the official [User Interaction Guide](https://cmake.org/cmake/help/latest/guide/user-interaction/index.html).

Interfaces
----------

The main interface is `RanluxppEngine` in the corresponding header file.
It implements the recommended seeding method and uses a luxury level of `p = 2048`.
Each random number is 48 bits wide and can optionally be returned as a `double`.

The file `RanluxppCompatEngine.h` provides generators that reproduce the same sequences as original RANLUX implementations.
The returned numbers have inferior quality, oftentimes only 24 bits wide and much lower luxury levels.
The generators are of HISTORICAL interest only, and SHOULD NOT be used for new applications!

If enabled with `-DRANLUXPP_GSL_INTERFACE=ON`, there is a third library with an interface for the [GNU Scientific Library](https://www.gnu.org/software/gsl/).
To use it, just declare

```c
extern const gsl_rng_type *gsl_rng_ranluxpp;
```

in your application and use it to [generate random numbers](https://www.gnu.org/software/gsl/doc/html/rng.html).

License
-------

```
Copyright (C) 2020-2021  CERN, Jonas Hahnfeld

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
```

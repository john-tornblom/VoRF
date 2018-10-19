# VoRF (Verifier of Random Forests)
VoRF (Verifier of Random Forests) is a toolsuite for analyzing input/output
mappings of decision trees and random forests. VoRF consists of two distinct
type of components, VoRF Core and VoRF Property Checker. VoRF Core takes as
input a random forest with corresponding input domain, and emits a sequence of
equivalence classes, i.e. sets of points in the input domain that yield the
same output. These equivalence classes are then processed by a VoRF Property
Checker that checks that all input/output mappings captured by an equivalence
class complies with some requirement.

For more information, see
[our paper](https://www.ida.liu.se/labs/rtslab/publications/2018/John_FTSCS.pdf).

## Building
Autotools is used to generate makefiles, and VoRF Core depends on libjson-c to
parse decision trees and random forests. On Ubuntu-flavored operating systems,
you can invoke the following commands to install dependencies, generate
makefiles, and compile the source code.
```console
john@localhost:VoRF$ sudo apt-get install build-essential libjson-c-dev
john@localhost:VoRF$ ./bootstrap.sh
john@localhost:VoRF$ ./configure
john@localhost:VoRF$ make
```

VoRF also includes Python bindings for easy prototyping of domain-specific
property checkers. See [test.py](bindings/python/test.py) for a simple example.
To build the bindings on Ubuntu-flavored operating systems, invoke the following
commands.
```console
john@localhost:VoRF$ sudo apt-get install python-cffi python-dev
john@localhost:VoRF$ python bindings/python/build.py
```

## Running Case Studies
VoRF includes example case studies that use scikit-learn to train random forests,
and VoRF to verify requirements. To install scikit-learn on Ubuntu-flavored
operating systems, invoke the following command:
```console
john@localhost:VoRF$ sudo apt-get install python-sklearn
```

To run a collision detection verification case study, invoke the following
command:
```console
john@localhost:VoRF$ ./support/collision_detection.sh
```

## Reporting Bugs
If you encounter problems with VoRF, please 
[file a github issue](https://github.com/john-tornblom/vorf/issues/new). 
If you plan on sending pull requests which affect more than a few lines of code, 
please file an issue before you start to work on you changes. This will allow us
to discuss the solution properly before you commit time and effort.

## Citation
If you use VoRF in a scientific publication, we would appreciate citations.
```
@inproceedings{Tornblom18,
  title = {Formal Verification of Random Forests in Safety-Critical Applications},
  author = {John Törnblom and Simin Nadjm-Tehrani},
  booktitle = {International Workshop on Formal Techniques for Safety-Critical Systems (FTSCS)},
  year = {2018},
  publisher = {Springer}
}
```

## License
[VoRF Core](lib) is licensed under the LGPLv3+, and [programs](src) linking to
VoRF Core are licensed under the GPLv3+. see COPYING and COPYING.LESSER for more
information.

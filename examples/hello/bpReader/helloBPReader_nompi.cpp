/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloBPReader.cpp: Simple self-descriptive example of how to read a variable
 * from a BP File.
 *
 *  Created on: Feb 16, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <ios>       //std::ios_base::failure
#include <iostream>  //std::cout
#include <fstream>   //std::fstream
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

int main(int argc, char *argv[])
{
    // Test if the file has been written by hello_bpWriter:
    std::string filename = "myVector_cpp.bp";
    std::ifstream f(filename.c_str());
    if (!f.good())
    {
       std::cerr << "The file " << filename << " does not exist."
                 << " Presumably this is because hello_bpWriter has not been run. Run ./hello_bpWriter before running this program.\n";
       return 1;
    }
    else
    {
       f.close();
    }


    try
    {
        /** ADIOS class factory of IO class objects, DebugON is recommended */
        adios2::ADIOS adios(adios2::DebugON);

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO bpIO = adios.DeclareIO("ReadBP");

        /** Engine derived class, spawned to start IO operations */
        adios2::Engine bpReader =
            bpIO.Open(filename.c_str(), adios2::Mode::Read);

        /** Write variable for buffering */
        adios2::Variable<float> bpFloats =
            bpIO.InquireVariable<float>("bpFloats");

        adios2::Variable<int> bpInts = bpIO.InquireVariable<int>("bpInts");

        if (bpFloats)
        {
            std::vector<float> myFloats;
            bpReader.Get<float>(bpFloats, myFloats, adios2::Mode::Sync);
            std::cout << "Float vector inside " << filename << ": {";
            for (auto & x : myFloats) {
              std::cout << x << ", ";
            }
            std::cout << "}\n";
        }

        if (bpInts)
        {
            std::vector<int> myInts;
            bpReader.Get<int>(bpInts, myInts, adios2::Mode::Sync);
        }
        else
        {
            std::cout << "There are no integer datasets in " << filename << ".\n";
        }

        /** Close bp file, engine becomes unreachable after this*/
        bpReader.Close();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "IO System base failure exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }

    return 0;
}

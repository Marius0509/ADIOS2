#include <adios2.h>
#include <vector>
#include <iostream>
#include <ctime>

const std::string output = "example3.bp";
constexpr int N = 100, T = 100;
const std::string extent = "0 " + std::to_string(N) + " 0 " + std::to_string(N) + " 0 " + std::to_string(N);

int main() {
    srand(time(0));

    adios2::ADIOS adios;
    adios2::IO io = adios.DeclareIO("IO");
    io.SetEngine("BP");
    io.DefineAttribute<std::string>("vtk.xml", R"(
<?xml version="1.0"?>
<VTKFile type="ImageData" version="0.1" byte_order="LittleEndian">
  <ImageData WholeExtent=")" + extent + R"(" Origin="0 0 0" Spacing="1 1 1">
    <Piece Extent=")" + extent + R"(">
        <CellData>
            <DataArray type="Int32" Name="scalars" format="ascii" />
            <DataArray Name="TIME">
                step
            </DataArray>
        </CellData>
    </Piece>
  </ImageData>
</VTKFile>
    )");
    adios2::Engine writer = io.Open(output, adios2::Mode::Write);

    adios2::Variable<int> varScalars = io.DefineVariable<int>("scalars", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varStep = io.DefineVariable<int>("step");

    std::vector<int> vecScalars(N * N * N);
    for (int t = 0; t < T; t++) {
        writer.BeginStep();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    int index = i * N * N + j * N + k;
                    vecScalars[index] = rand();
                }
            }
        }
        writer.Put(varScalars, vecScalars.data());
        writer.Put(varStep, t);
        writer.EndStep();
        std::cout << t + 1 << '/' << T << '\n';
    }
    
    writer.Close();
    std::cout << "Data written to " << output << '\n';
    return 0;
}

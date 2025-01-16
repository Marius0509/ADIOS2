#include <adios2.h>
#include <vector>
#include <iostream>

const std::string output = "example1.bp";
constexpr int N = 50;
const std::string extent = "0 " + std::to_string(N) + " 0 " + std::to_string(N) + " 0 " + std::to_string(N);

int main() {
    adios2::ADIOS adios;
    adios2::IO io = adios.DeclareIO("IO");
    io.SetEngine("BP");
    io.DefineAttribute<std::string>("vtk.xml", R"(
<?xml version="1.0"?>
<VTKFile type="ImageData" version="0.1" byte_order="LittleEndian">
  <ImageData WholeExtent=")" + extent + R"(" Origin="0 0 0" Spacing="1 1 1">
    <Piece Extent=")" + extent + R"(">
        <CellData>
            <DataArray type="Int32" Name="i" format="ascii" />
            <DataArray type="Int32" Name="j" format="ascii" />
            <DataArray type="Int32" Name="k" format="ascii" />
            <DataArray type="Int32" Name="ij" format="ascii" />
            <DataArray type="Int32" Name="ik" format="ascii" />
            <DataArray type="Int32" Name="jk" format="ascii" />
            <DataArray type="Int32" Name="ijk" format="ascii" />
        </CellData>
    </Piece>
  </ImageData>
</VTKFile>
    )");
    adios2::Engine writer = io.Open(output, adios2::Mode::Write);

    adios2::Variable<int> varI = io.DefineVariable<int>("i", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varJ = io.DefineVariable<int>("j", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varK = io.DefineVariable<int>("k", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varIJ = io.DefineVariable<int>("ij", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varIK = io.DefineVariable<int>("ik", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varJK = io.DefineVariable<int>("jk", {N, N, N}, {0, 0, 0}, {N, N, N});
    adios2::Variable<int> varIJK = io.DefineVariable<int>("ijk", {N, N, N}, {0, 0, 0}, {N, N, N});

    std::vector<int> vecI(N * N * N), vecJ(N * N * N), vecK(N * N * N), vecIJ(N * N * N), vecIK(N * N * N), vecJK(N * N * N), vecIJK(N * N * N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                int index = i * N * N + j * N + k;
                vecI[index] = i;
                vecJ[index] = j;
                vecK[index] = k;
                vecIJ[index] = i + j;
                vecIK[index] = i + k;
                vecJK[index] = j + k;
                vecIJK[index] = i + j + k;
            }
        }
    }

    writer.Put(varI, vecI.data());
    writer.Put(varJ, vecJ.data());
    writer.Put(varK, vecK.data());
    writer.Put(varIJ, vecIJ.data());
    writer.Put(varIK, vecIK.data());
    writer.Put(varJK, vecJK.data());
    writer.Put(varIJK, vecIJK.data());
    
    writer.Close();
    std::cout << "Data written to " << output << '\n';
    return 0;
}

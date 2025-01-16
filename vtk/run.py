import vtk

FILENAME = 'test.vtu'
extension = FILENAME.split('.')[-1]
if extension == 'vti':
    reader = vtk.vtkXMLImageDataReader()
elif extension == 'vtu':
    reader = vtk.vtkXMLUnstructuredGridReader()
else:
    print(f'unknown extension {extension}')
    exit()
reader.SetFileName(FILENAME)
mapper = vtk.vtkDataSetMapper()
mapper.SetInputConnection(reader.GetOutputPort())
actor = vtk.vtkActor()
actor.SetMapper(mapper)
renderer = vtk.vtkRenderer()
renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)
renderer.AddActor(actor)
renderer.SetBackground(1.0, 1.0, 1.0)
renderWindow.Render()
renderWindowInteractor.Start()
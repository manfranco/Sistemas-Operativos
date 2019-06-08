all: src/DeleteEvaluador.exe src/Evaluador.exe src/ingresoelemento.exe src/iniciadorMemoria.exe src/Reporte.exe src/Salida.exe

LDFLAGS= -lrt -pthread

src/DeleteEvaluador.exe: src/DeleteEvaluador.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/Evaluador.exe: src/Evaluador.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/ingresoelemento.exe: src/ingresoelemento.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/iniciadorMemoria.exe: src/iniciadorMemoria.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/Reporte.exe: src/Reporte.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/Salida.exe: src/Salida.o
	$(CXX) -o $@ $^ $(LDFLAGS)

src/DeleteEvaluador.o: src/DeleteEvaluador.cpp src/elementos.h

src/Evaluador.o: src/Evaluador.cpp src/elementos.h

src/ingresoelemento.o: src/ingresoelemento.cpp src/elementos.h

src/iniciadorMemoria.o: src/iniciadorMemoria.cpp src/elementos.h

src/Reporte.o: src/Reporte.cpp src/elementos.h

src/Salida.o: src/Salida.cpp src/elementos.h

clean:
	rm -f src/*.o
	rm -f src/*.*~ *~
	rm -f src/DeleteEvaluador.exe src/Evaluador.exe src/ingresoelemento.exe src/iniciadorMemoria.exe src/Reporte.exe src/Salida.exe
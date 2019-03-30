#ifndef matrix_h
#define matrix_h

class matrixclass
{
  public: //User accessible public interface
    matrixclass();
    void SETUP();
    void matrix();
	void completePass(int route[][2], int length);
	void beginRoute(int startPoint); //not sure how this works fully need to fix
};

//extern matrixclass matrix;
  
  #endif
  

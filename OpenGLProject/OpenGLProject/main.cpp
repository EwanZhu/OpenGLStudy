#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点Shader
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
    "   vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
    "}\n";
// 片断Shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vertexColor;\n"
//    "   FragColor = vec4(0.0,0.0,0.9,1.0);\n"
    "}\n";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //设置窗口的OpenGL主版本号，由于openGL库版本号为3.3所以大版本为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //设置窗口的OpenGL副版本号，由于openGL库版本号为3.3所以小版本为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置窗口的OpenGL模式为Core模式
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //设置窗口是否可以更变大小

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();//中止释放glfw
        return -1;
    }
    glfwMakeContextCurrent(window); //通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建着色器。
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);//将着色器源码附加到着色器对象上。
    glCompileShader(vertexShader);//编译着色器。
    
    // 测试顶点Shader编译是否通过。
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout <<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }else{
        std::cout <<"SUCCESS::SHADER::VERTEX::COMPILATION_COMPLATED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    // 测试片断Shader编译是否通过。
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout <<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }else{
        std::cout <<"SUCCESS::SHADER::FRAGMENT::COMPILATION_COMPLATED\n" << infoLog << std::endl;
    }
    
    //创建着色器程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    //检测连接着色器t程度是否失败
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout <<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }else{
         std::cout <<"SUCCESS::SHADER::PROGRAM::LINKING_COMPLATED\n" << infoLog << std::endl;
    }
    
    //删除顶点和片断Shader，不再需要
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //定义顶点的数据
    float vertices[] ={
        -0.5f,-0.5f,0.0f,
        -0.5f,0.5f,0.0f,
        0.5f,0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.8f,-0.5f,0.0f,
        0.8f,0.5f,0.0f
    };
    
    unsigned int indices[] = {
        0,1,3,
        1,2,3,
        3,2,4,
        2,5,4
    };
    
    // 顶点缓冲对象(Vertex Buffer Objects, VBO)
    // 顶点数组对象(Vertex Array Object, VAO)
    // 索引缓冲对象(Element Buffer Object, EBO)
    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    
    // --0-- 复制顶点数组到缓冲中供OpenGL使用
    // 使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    // GL_STANTIC_DRAW：数据不会或几乎不会改变。
    // GL_DYNAMIC_DRAW：数据会被改变很多。
    // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    // glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    // --1-- 设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    // 渲染循环 loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input输入
        // -----
        processInput(window);

        // render渲染
        // ------
        glClearColor(0.2f, 0.5f, 0.1f, 1.0f);//清除颜色，否则会留上一帧的画面
        glClear(GL_COLOR_BUFFER_BIT);//清除缓存
        
      
        // 使用着色器程序
        glUseProgram(shaderProgram);
        
        // 更新uniform颜色
        float timeValue = glfwGetTime();//获取运行的秒数
        float greenValue = (sin(timeValue)/2.0f)+0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram,"vertexColor"); //获得位置值索引，如果该函数返回-1表示没有找到位置值
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f); //通过获取到的位置值为其赋值，该赋值必须在glUseProgram函数后，因为这个设置是作用于激活后的着色器程序所设置的uniform。
        
        // 绑定顶点
        glBindVertexArray(VAO);
        // 绘制物体
        //glDrawArrays(GL_TRIANGLES,0,3);当绘制三角形时被替换
        glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//以线框模式绘制
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0.2,0.2, width, height);
}




#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class ModernOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vbo{ QOpenGLBuffer::VertexBuffer };
    QOpenGLVertexArrayObject vao;

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();

        // Load shaders from file
        if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.glsl"))
            qWarning() << "Vertex shader error:" << shaderProgram.log();

        if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragment.glsl"))
            qWarning() << "Fragment shader error:" << shaderProgram.log();

        if (!shaderProgram.link())
            qWarning() << "Shader link error:" << shaderProgram.log();

        float vertices[] = {
            0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
           -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f
        };

        vao.create();
        vao.bind();

        vbo.create();
        vbo.bind();
        vbo.allocate(vertices, sizeof(vertices));

        shaderProgram.bind();
        shaderProgram.enableAttributeArray(0);
        shaderProgram.enableAttributeArray(1);
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
        shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));

        vbo.release();
        vao.release();
        shaderProgram.release();

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);

        QMatrix4x4 projection;
        float aspect = float(w) / float(h ? h : 1);
        projection.ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f); // maintain square aspect

        shaderProgram.bind();
        shaderProgram.setUniformValue("projection", projection);
        shaderProgram.release();
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.bind();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao.release();
        shaderProgram.release();
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ModernOpenGLWidget window;
    window.resize(600, 400);
    window.setWindowTitle("Modern OpenGL (Shaders from Files)");
    window.show();

    return app.exec();
}
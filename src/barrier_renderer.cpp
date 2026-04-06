#include "barrier_renderer.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void BarrierRenderer::drawBarrierSurface(float cutoff_z_barrier, unsigned int texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0.f, 0.f);
            glVertex3f(50.0f, 10.0f, cutoff_z_barrier);
            glTexCoord2f(5.f, 0.f);
            glVertex3f(-50.0f, 10.0f, cutoff_z_barrier);
            glTexCoord2f(5.f, 1.f);
            glVertex3f(-50.0f, 15.0f, cutoff_z_barrier);
            glTexCoord2f(0.f, 1.f);
            glVertex3f(50.0f, 15.0f, cutoff_z_barrier);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void BarrierRenderer::drawBarrierBody(float cutoff_z_barrier, unsigned int texture_id) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0.f, 0.f);
            glVertex3f(-45.0f, 0.0f, cutoff_z_barrier - 1.0f);
            glTexCoord2f(1.f, 0.f);
            glVertex3f(-55.0f, 0.0f, cutoff_z_barrier - 1.0f);
            glTexCoord2f(1.f, 1.f);
            glVertex3f(-55.0f, 15.0f, cutoff_z_barrier - 1.0f);
            glTexCoord2f(0.f, 1.f);
            glVertex3f(-45.0f, 15.0f, cutoff_z_barrier - 1.0f);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void BarrierRenderer::drawTrafficLight(bool stop_light, float cutoff_z_barrier) {
    if (stop_light) {
        glColor3f(1.0f, 0.1f, 0.1f);
    } else {
        glColor3f(0.0f, 1.0f, 0.1f);
    }

    glPushMatrix();
        glTranslatef(-50.0f, 15.0f, cutoff_z_barrier);
        glutSolidSphere(3.0, 20.0, 20.0);
    glPopMatrix();
}

void BarrierRenderer::drawAnimatedBarrierArm(float barrier_theta, float cutoff_z_barrier, unsigned int texture_id) {
    glPushMatrix();
        glTranslatef(-50.0f, 10.0f, cutoff_z_barrier);
        glRotatef(barrier_theta, 0.0f, 0.0f, 1.0f);
        glTranslatef(50.0f, -10.0f, -cutoff_z_barrier);
        glVertex3f(-50.0f, 10.0f, cutoff_z_barrier);
        drawBarrierSurface(cutoff_z_barrier, texture_id);
    glPopMatrix();
}

//
// Created by matteo on 01/03/19.
//

#include <DirectedPartitioner.h>
#include <stack>
#include <boost/circular_buffer.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <partitionTypes.h>
#include <stopwatch.h>

struct MyPosData {
    GLdouble x = 0;
    GLdouble y = 0;
    GLdouble scale = 1;
    GLdouble translationX = 0;
    GLdouble translationY = 0;
    bool to_display = false;
};

void controlss(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        MyPosData *dataPtr = (MyPosData*)glfwGetWindowUserPointer(window);
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_SPACE:
                dataPtr->to_display = false;
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                dataPtr->scale += 0.5;
                break;
            case GLFW_KEY_P:
                dataPtr->scale -= 0.5;
                break;
            case GLFW_KEY_A:
                dataPtr->translationX -= 0.05;
                break;
            case GLFW_KEY_D:
                dataPtr->translationX += 0.05;
                break;
            case GLFW_KEY_W:
                dataPtr->translationY += 0.05;
                break;
            case GLFW_KEY_S:
                dataPtr->translationY -= 0.05;
                break;
        }
    }
    if(action == GLFW_REPEAT) {
        MyPosData *dataPtr = (MyPosData*)glfwGetWindowUserPointer(window);
        switch (key) {
            case GLFW_KEY_RIGHT:
                dataPtr->x -= 1;
                break;
            case GLFW_KEY_LEFT:
                dataPtr->x += 1;
                break;
            case GLFW_KEY_DOWN:
                dataPtr->y -= 1;
                break;
            case GLFW_KEY_UP:
                dataPtr->y += 1;
                break;
        }
    }
}

GLFWwindow* initWindoww(const int resX, const int resY)
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controlss);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

void drawCubee(GLFWwindow* window,std::vector<GLfloat> coords,std::vector<GLfloat> colors)
{
    MyPosData *dataPtr = (MyPosData*)glfwGetWindowUserPointer(window);

    glRotatef(dataPtr->x, 0, 1, 0);
    glRotatef(dataPtr->y, 1, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, coords.data());
    glColorPointer(3, GL_FLOAT, 0, colors.data());

    int size = coords.size();
    glScalef(dataPtr->scale, dataPtr->scale, dataPtr->scale);
    glTranslatef(dataPtr->translationX, dataPtr->translationY, 0);
    glDrawArrays(GL_TRIANGLES, 0, coords.size() / 3);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

std::vector<unsigned int> DirectedPartitioner::partitionMesh(const Mesh &mesh)
{
    Stopwatch stopwatch;
    stopwatch.start();
    DirectedPartition directedPartition;
    directedPartition.mesh = mesh;
    directedPartition.isFaceInPartition = new std::vector<bool>(mesh.num_faces(),true);
    directedPartition.isFaceExplored = new std::vector<bool>(mesh.num_faces(),false);
    directedPartition.isFaceVisited = new std::vector<bool>(mesh.num_faces(),false);
    directedPartition.initDsc = Mesh::face_index(0);
    directedPartition.cutThresh = mesh.num_faces()/2;

    bisectMesh(directedPartition);
    std::cout << stopwatch.stop() << std::endl;

    GLFWwindow* window = initWindoww(1024, 620);
    MyPosData my_pos_data;
    glfwSetWindowUserPointer(window,&my_pos_data);
    std::vector<GLfloat> coords,colors;

    CGAL::Vertex_around_face_iterator<Mesh> vafb,vafe;
    for(int i = 0; i < directedPartition.isFaceVisited->size(); ++i) {
        if(directedPartition.isFaceVisited->operator[](i) && !directedPartition.isFaceExplored->operator[](i)) {
            for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                Point3 p = (Point3) mesh.point(*vafb);
                coords.push_back(p.x());
                coords.push_back(p.y());
                coords.push_back(p.z());

                colors.push_back(0.0);
                colors.push_back(1.0);
                colors.push_back(0.0);
            }
        }
    }

    for(int i = 0; i < directedPartition.isFaceExplored->size(); ++i) {
        if(directedPartition.isFaceExplored->operator[](i)) {
            for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                Point3 p = (Point3) mesh.point(*vafb);
                coords.push_back(p.x());
                coords.push_back(p.y());
                coords.push_back(p.z());

                colors.push_back(1.0);
                colors.push_back(0.0);
                colors.push_back(0.0);
            }
        }
    }

    my_pos_data.to_display = true;
    while (my_pos_data.to_display) {
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw stuff
        glClearColor(0.9, 0.9, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();

        gluPerspective(60, (double) windowWidth / (double) windowHeight, 0.1, 100);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0, 0, -5);

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        drawCubee(window,coords,colors);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();

    }
    glfwDestroyWindow(window);
}

void DirectedPartitioner::bisectMesh(DirectedPartition &dirPart)
{
    std::stack<Mesh::face_index> faceDscStack;
    std::unordered_map<Mesh::face_index,Mesh::face_index> originOfFaceDsc;
    bool isSenseCCW = true;
    unsigned int faceCount = 1;

    CGAL::Face_around_face_iterator<Mesh> fafib, fafie;
    for (boost::tie(fafib, fafie) = CGAL::faces_around_face(dirPart.mesh.halfedge(dirPart.initDsc),dirPart.mesh); fafib != fafie; ++fafib)
    {
        if(*fafib != Mesh::null_face() && dirPart.isFaceInPartition->operator[](*fafib))
        {
            faceDscStack.emplace(*fafib);
            originOfFaceDsc.insert({*fafib,dirPart.initDsc});
            dirPart.isFaceVisited->operator[](*fafib) = true;
        }
    }
    dirPart.isFaceVisited->operator[](dirPart.initDsc) = true;
    dirPart.isFaceExplored->operator[](dirPart.initDsc) = true;

    while(!faceDscStack.empty())
    {
        Mesh::face_index currFaceDsc = faceDscStack.top();
        faceDscStack.pop();
        dirPart.isFaceExplored->operator[](currFaceDsc) = true;
        faceCount++;

        std::vector<Mesh::face_index> currFaceNeighs;
        unsigned short currNeighIdx = 0;
        short originIdx = -1;
        short nullFaceIdx = -1;
        for (boost::tie(fafib,fafie) = CGAL::faces_around_face(dirPart.mesh.halfedge(currFaceDsc),dirPart.mesh); fafib != fafie; ++fafib)
        {
            if(*fafib != Mesh::null_face() && dirPart.isFaceInPartition->operator[](*fafib))
            {
                currFaceNeighs.emplace_back(*fafib);

                if(*fafib == originOfFaceDsc[currFaceDsc])
                {
                    originIdx = currNeighIdx;
                }
            }
            else
            {
                nullFaceIdx = currNeighIdx;
            }

            currNeighIdx++;
        }

        if(currFaceNeighs.size() == 2)
        {
            if(isSenseCCW)
            {
                bool cond1 = originIdx == 0 && nullFaceIdx == 2;
                bool cond2 = originIdx == 1 && nullFaceIdx == 0;
                bool cond3 = originIdx == 2 && nullFaceIdx == 1;

                if(cond1 || cond2 || cond3)
                    isSenseCCW = false;
            }
            else
            {
                bool cond1 = originIdx == 0 && nullFaceIdx == 1;
                bool cond2 = originIdx == 1 && nullFaceIdx == 2;
                bool cond3 = originIdx == 2 && nullFaceIdx == 0;

                if(cond1 || cond2 || cond3)
                    isSenseCCW = true;
            }
        }

        boost::circular_buffer<Mesh::face_index> currFaceCircularNeighs{currFaceNeighs.size()};
        currFaceCircularNeighs.assign(currFaceNeighs.begin(),currFaceNeighs.end());

        Mesh::face_index initiatorFace = originOfFaceDsc[currFaceDsc];
        while (*currFaceCircularNeighs.begin() != initiatorFace)
        {
            currFaceCircularNeighs.rotate(currFaceCircularNeighs.begin() + 1);
        }

        if(isSenseCCW)
        {
            for(unsigned int i = 1; i < currFaceCircularNeighs.size(); ++i)
            {
                Mesh::face_index currNeigh = currFaceCircularNeighs[i];
                if(!dirPart.isFaceExplored->operator[](currNeigh))
                {
                    faceDscStack.emplace(currNeigh);
                    dirPart.isFaceVisited->operator[](currNeigh) = true;
                    originOfFaceDsc[currNeigh] = currFaceDsc;
                }
            }
        }
        else
        {
            for(unsigned int i = currFaceCircularNeighs.size() - 1; i > 0; --i)
            {
                Mesh::face_index currNeigh = currFaceCircularNeighs[i];
                if(!dirPart.isFaceExplored->operator[](currNeigh))
                {
                    faceDscStack.emplace(currNeigh);
                    dirPart.isFaceVisited->operator[](currNeigh) = true;
                    originOfFaceDsc[currNeigh] = currFaceDsc;
                }
            }
        }

        if(faceCount > dirPart.cutThresh)
        {
            dirPart.lastFaceDsc = currFaceDsc;
            break;
        }
    }
}

std::vector<bool> DirectedPartitioner::bisectMesh(const Mesh &mesh,const Mesh::face_index initDsc, const std::vector<bool> &isNodeInSubMesh)
{
    Stopwatch stopwatch;
    stopwatch.start();
    std::stack<Mesh::face_index> faceDscStack;
    std::vector<bool> isFaceVisited(mesh.num_faces(),false);
    std::vector<bool> isFaceExplored(mesh.num_faces(),false);
    std::unordered_map<Mesh::face_index,Mesh::face_index> originFaces;
    bool isSenseCCW = true;
    unsigned int faceCount = 1;

    // Handle the initial node
    CGAL::Face_around_face_iterator<Mesh> fafib, fafie;
    for (boost::tie(fafib, fafie) = CGAL::faces_around_face(mesh.halfedge(initDsc),mesh); fafib != fafie; ++fafib)
    {
        if(*fafib != Mesh::null_face() && isNodeInSubMesh[*fafib])
        {
            faceDscStack.emplace(*fafib);
            originFaces.insert({*fafib, initDsc});
            isFaceVisited[*fafib] = true;
        }
    }
    isFaceExplored[initDsc];
    isFaceVisited[initDsc];
    int sum = 0;
    for(auto el : isNodeInSubMesh)
        if(el) sum++;
    int thresh = sum/2;

    // Handle the remaining nodes
    while(!faceDscStack.empty())
    {
        Mesh::face_index currFaceDsc = faceDscStack.top();
        faceDscStack.pop();

        isFaceExplored[currFaceDsc] = true;
        faceCount++;
        //std::cout << currFaceDsc << std::endl;

        // ABOVE THRESHOLD
        if(faceCount > thresh)
        {
            std::cout << stopwatch.stop() << std::endl;
            GLFWwindow* window = initWindoww(1024, 620);
            MyPosData my_pos_data;
            glfwSetWindowUserPointer(window,&my_pos_data);
            std::vector<GLfloat> coords,colors;

            CGAL::Vertex_around_face_iterator<Mesh> vafb,vafe;
            /*for(int i = 0; i < isFaceVisited.size(); ++i) {
                if(isFaceVisited[i] && !isFaceExplored[i]) {
                    for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                        Point3 p = (Point3) mesh.point(*vafb);
                        coords.push_back(p.x());
                        coords.push_back(p.y());
                        coords.push_back(p.z());

                        colors.push_back(0.0);
                        colors.push_back(1.0);
                        colors.push_back(0.0);
                    }
                }
            }

           for(int i = 0; i < isFaceExplored.size(); ++i) {
                if(isFaceExplored[i]) {
                    for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                        Point3 p = (Point3) mesh.point(*vafb);
                        coords.push_back(p.x());
                        coords.push_back(p.y());
                        coords.push_back(p.z());

                        colors.push_back(1.0);
                        colors.push_back(0.0);
                        colors.push_back(0.0);
                    }
                }
            }*/

            Mesh::face_index initFaceDsc = findOuterBorderInit(currFaceDsc,originFaces[currFaceDsc],isNodeInSubMesh,isFaceExplored,mesh);
            /*for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(initFaceDsc),mesh);vafb != vafe;++vafb) {
                Point3 p = (Point3) mesh.point(*vafb);
                coords.push_back(p.x());
                coords.push_back(p.y());
                coords.push_back(p.z());

                colors.push_back(0.0);
                colors.push_back(0.0);
                colors.push_back(0.0);
            }*/

            std::vector<Mesh::face_index> hol = findOuterBorder(initFaceDsc,isNodeInSubMesh,isFaceExplored,mesh);
            /*for(auto el : hol)
            for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(el),mesh);vafb != vafe;++vafb) {
                Point3 p = (Point3) mesh.point(*vafb);
                coords.push_back(p.x());
                coords.push_back(p.y());
                coords.push_back(p.z());

                colors.push_back(0.0);
                colors.push_back(0.0);
                colors.push_back(1.0);
            }*/

            /*for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(initFaceDsc),mesh);vafb != vafe;++vafb) {
                Point3 p = (Point3) mesh.point(*vafb);
                coords.push_back(p.x());
                coords.push_back(p.y());
                coords.push_back(p.z());

                colors.push_back(0.0);
                colors.push_back(0.0);
                colors.push_back(0.0);
            }*/

            std::vector<Mesh::face_index> iop = findHoles(isFaceVisited,isFaceExplored,hol);
            /*for(auto el : iop)
                for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(el),mesh);vafb != vafe;++vafb) {
                    Point3 p = (Point3) mesh.point(*vafb);
                    coords.push_back(p.x());
                    coords.push_back(p.y());
                    coords.push_back(p.z());

                    colors.push_back(0.0);
                    colors.push_back(0.0);
                    colors.push_back(0.0);
                }*/

            fillHoles(mesh,iop,isFaceExplored,isNodeInSubMesh);
            /*for(int i = 0; i < isFaceExplored.size(); ++i) {
                if(isFaceExplored[i]) {
                    for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                        Point3 p = (Point3) mesh.point(*vafb);
                        coords.push_back(p.x());
                        coords.push_back(p.y());
                        coords.push_back(p.z());

                        colors.push_back(1.0);
                        colors.push_back(0.0);
                        colors.push_back(0.0);
                    }
                } else {
                    for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                        Point3 p = (Point3) mesh.point(*vafb);
                        coords.push_back(p.x());
                        coords.push_back(p.y());
                        coords.push_back(p.z());

                        colors.push_back(0.0);
                        colors.push_back(1.0);
                        colors.push_back(0.0);
                    }
                }
            }*/

            unravelPartition(currFaceDsc,originFaces[currFaceDsc],mesh,isFaceExplored,isNodeInSubMesh);
            for(int i = 0; i < isFaceExplored.size(); ++i) {
                if(isFaceExplored[i]) {
                    for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                        Point3 p = (Point3) mesh.point(*vafb);
                        coords.push_back(p.x());
                        coords.push_back(p.y());
                        coords.push_back(p.z());

                        colors.push_back(1.0);
                        colors.push_back(0.0);
                        colors.push_back(0.0);
                    }
                } else {
                    for(boost::tie(vafb,vafe) = CGAL::vertices_around_face(mesh.halfedge(Mesh::face_index(i)),mesh);vafb != vafe;++vafb) {
                        Point3 p = (Point3) mesh.point(*vafb);
                        coords.push_back(p.x());
                        coords.push_back(p.y());
                        coords.push_back(p.z());

                        colors.push_back(0.0);
                        colors.push_back(0.0);
                        colors.push_back(1.0);
                    }
                }}

            my_pos_data.to_display = true;
            while (my_pos_data.to_display) {
                GLint windowWidth, windowHeight;
                glfwGetWindowSize(window, &windowWidth, &windowHeight);
                glViewport(0, 0, windowWidth, windowHeight);

                // Draw stuff
                glClearColor(0.9, 0.9, 0.8, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glMatrixMode(GL_PROJECTION_MATRIX);
                glLoadIdentity();

                gluPerspective(60, (double) windowWidth / (double) windowHeight, 0.1, 100);

                glMatrixMode(GL_MODELVIEW_MATRIX);
                glTranslatef(0, 0, -5);

                //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                drawCubee(window,coords,colors);
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

                // Update Screen
                glfwSwapBuffers(window);

                // Check for any input, or window movement
                glfwPollEvents();

            }
            glfwDestroyWindow(window);

            break;
        }

        std::vector<Mesh::face_index> currFaceNeighs;
        unsigned short currNeighIdx = 0;
        short originIdx = -1;
        short nullFaceIdx = -1;
        for (boost::tie(fafib,fafie) = CGAL::faces_around_face(mesh.halfedge(currFaceDsc),mesh); fafib != fafie; ++fafib)
        {
            if(*fafib != Mesh::null_face())
            {
                if (isNodeInSubMesh[*fafib])
                {
                    currFaceNeighs.emplace_back(*fafib);

                    if(*fafib == originFaces[currFaceDsc])
                    {
                        originIdx = currNeighIdx;
                    }
                }
                else
                {
                    nullFaceIdx = currNeighIdx;
                }
            }
            else
            {
                nullFaceIdx = currNeighIdx;
            }

            currNeighIdx++;
        }

        if(currFaceNeighs.size() == 2)
        {
            if(isSenseCCW)
            {
                bool cond1 = originIdx == 0 && nullFaceIdx == 2;
                bool cond2 = originIdx == 1 && nullFaceIdx == 0;
                bool cond3 = originIdx == 2 && nullFaceIdx == 1;

                if(cond1 || cond2 || cond3)
                    isSenseCCW = false;
            }
            else
            {
                bool cond1 = originIdx == 0 && nullFaceIdx == 1;
                bool cond2 = originIdx == 1 && nullFaceIdx == 2;
                bool cond3 = originIdx == 2 && nullFaceIdx == 0;

                if(cond1 || cond2 || cond3)
                    isSenseCCW = true;
            }
        }

        boost::circular_buffer<Mesh::face_index> currFaceCircularNeighs{currFaceNeighs.size()};
        currFaceCircularNeighs.assign(currFaceNeighs.begin(),currFaceNeighs.end());

        Mesh::face_index initiatorFace = originFaces[currFaceDsc];
        while (*currFaceCircularNeighs.begin() != initiatorFace)
        {
            currFaceCircularNeighs.rotate(currFaceCircularNeighs.begin() + 1);
        }

        if(isSenseCCW)
        {
            for(unsigned int i = 1; i < currFaceCircularNeighs.size(); ++i)
            {
                Mesh::face_index currNeigh = currFaceCircularNeighs[i];
                if(!isFaceExplored[currNeigh])
                {
                    faceDscStack.emplace(currNeigh);
                    isFaceVisited[currNeigh] = true;
                    originFaces[currNeigh] = currFaceDsc;
                }
            }
        }
        else
        {
            for(unsigned int i = currFaceCircularNeighs.size() - 1; i > 0; --i)
            {
                Mesh::face_index currNeigh = currFaceCircularNeighs[i];
                if(!isFaceExplored[currNeigh])
                {
                    faceDscStack.emplace(currNeigh);
                    isFaceVisited[currNeigh] = true;
                    originFaces[currNeigh] = currFaceDsc;
                }
            }
        }
    }

    return isFaceExplored;
}

Mesh::face_index DirectedPartitioner::findOuterBorderInit(const Mesh::face_index lastFaceDsc,
                                                        const Mesh::face_index lastButOneFaceDsc,
                                                        const std::vector<bool> &isNodeInSubMesh,
                                                        const std::vector<bool> &isFaceExplored,
                                                        const Mesh &mesh)
{
    // Explore until first element of the other border is found
    std::stack<Mesh::face_index> faceDscStack;
    std::unordered_map<Mesh::face_index,Mesh::face_index> originFaces;
    std::unordered_set<Mesh::face_index> isFaceVisited;
    bool exitCond = false;
    Mesh::face_index otherPartFaceDsc;

    faceDscStack.emplace(lastButOneFaceDsc);
    originFaces[lastButOneFaceDsc] = lastFaceDsc;

    CGAL::Face_around_face_iterator<Mesh> fafib, fafie;
    while(!exitCond)
    {
        Mesh::face_index currFaceDsc = faceDscStack.top();
        faceDscStack.pop();
        isFaceVisited.insert(currFaceDsc);

        std::vector<Mesh::face_index> currFaceNeighs;
        for (boost::tie(fafib,fafie) = CGAL::faces_around_face(mesh.halfedge(currFaceDsc),mesh); fafib != fafie; ++fafib)
        {
            if(*fafib != Mesh::null_face() && isNodeInSubMesh[*fafib])
            {
                currFaceNeighs.emplace_back(*fafib);
            }
        }

        boost::circular_buffer<Mesh::face_index> currFaceCircularNeighs(currFaceNeighs.size());
        currFaceCircularNeighs.assign(currFaceNeighs.begin(),currFaceNeighs.end());

        Mesh::face_index currFaceOriginDsc = originFaces[currFaceDsc];
        while(*currFaceCircularNeighs.begin() != currFaceOriginDsc)
        {
            currFaceCircularNeighs.rotate(currFaceCircularNeighs.begin() + 1);
        }

        for(unsigned int i = 0; i < currFaceCircularNeighs.size(); ++i)
        {
            if(isFaceExplored[currFaceCircularNeighs[i]])
            {
                if(isFaceVisited.find(currFaceCircularNeighs[i]).operator->() == isFaceVisited.end().operator->())
                {
                    faceDscStack.emplace(currFaceCircularNeighs[i]);
                }
            }
            else
            {
                exitCond = true;
                otherPartFaceDsc = currFaceCircularNeighs[i];
                break;
            }

            originFaces[currFaceCircularNeighs[i]] = currFaceDsc;
        }
    }

    return otherPartFaceDsc;
}

std::vector <Mesh::face_index> DirectedPartitioner::findOuterBorder(const Mesh::face_index initFaceDsc,
                                                                  const std::vector<bool> &isNodeInSubMesh,
                                                                  const std::vector<bool> &isFaceExplored,
                                                                  const Mesh &mesh)
{
    std::stack<Mesh::face_index> holeFacesStack;
    std::vector<bool> isOuterFaceInserted(mesh.num_faces(),false);

    holeFacesStack.emplace(initFaceDsc);
    isOuterFaceInserted[initFaceDsc] = true;

    while(!holeFacesStack.empty())
    {
        Mesh::face_index currFaceDsc = holeFacesStack.top();
        holeFacesStack.pop();

        CGAL::Face_around_face_iterator<Mesh> fafBegin, fafEnd;
        for (boost::tie(fafBegin, fafEnd) = CGAL::faces_around_face(mesh.halfedge(currFaceDsc), mesh); fafBegin != fafEnd; ++fafBegin)
        {
            if(*fafBegin != Mesh::null_face() && !isFaceExplored[*fafBegin] && isNodeInSubMesh[*fafBegin] && !isOuterFaceInserted[*fafBegin])
            {
                holeFacesStack.emplace(*fafBegin);
                isOuterFaceInserted[*fafBegin] = true;
            }
        }
    }

    std::vector<Mesh::face_index> oso;
    for(unsigned int i = 0; i < isOuterFaceInserted.size(); ++i)
    {
        if(isOuterFaceInserted[i]) oso.emplace_back(Mesh::face_index(i));
    }

    return oso;
   /* std::cout << initFaceDsc << std::endl;

    std::stack<Mesh::face_index> faceDscStack;
    std::unordered_map<Mesh::face_index,Mesh::face_index> originFaces;
    std::unordered_set<Mesh::face_index> visitedFaces;
    std::vector<bool> maramiao(mesh.num_faces(),false);
    bool exitCond = false;

    CGAL::Face_around_face_iterator<Mesh> fafib, fafie;
    unsigned short currFaceIdx = 0;
    unsigned short otherFaceIdx = 0;
    std::vector<Mesh::face_index> tmpNeighs;
    for (boost::tie(fafib, fafie) = CGAL::faces_around_face(mesh.halfedge(initFaceDsc),mesh); fafib != fafie; ++fafib)
    {
        if(*fafib != Mesh::null_face() && isNodeInSubMesh[*fafib])
        {
            if (!isFaceExplored[*fafib])
            {
                tmpNeighs.emplace_back(*fafib);
                originFaces[*fafib] = initFaceDsc;
            }
            else
            {
                otherFaceIdx = currFaceIdx;
            }
        }
        currFaceIdx++;
    }
    visitedFaces.insert(initFaceDsc);
    maramiao[initFaceDsc] = true;

    if(tmpNeighs.size() == 1)
    {
        faceDscStack.emplace(tmpNeighs[0]);
    }

    if(tmpNeighs.size() == 2)
    {
        if(otherFaceIdx == 1)
        {
            faceDscStack.emplace(tmpNeighs[1]);
            faceDscStack.emplace(tmpNeighs[0]);
        }
        else
        {
            faceDscStack.emplace(tmpNeighs[0]);
            faceDscStack.emplace(tmpNeighs[1]);
        }
    }
    int count = 0;
    while(true)
    {
        Mesh::face_index currFaceDsc = faceDscStack.top();
        faceDscStack.pop();
        maramiao[currFaceDsc] = true;
        //std::cout << currFaceDsc << std::endl;

        std::vector<Mesh::face_index> tmpNeighbours;
        for (boost::tie(fafib, fafie) = CGAL::faces_around_face(mesh.halfedge(currFaceDsc), mesh); fafib != fafie; ++fafib)
        {
            if(*fafib != Mesh::null_face() && isNodeInSubMesh[*fafib] && !isFaceExplored[*fafib])
            {
                tmpNeighbours.emplace_back(*fafib);

                if(maramiao[*fafib] && originFaces[currFaceDsc] != *fafib )//&& *fafib == initFaceDsc)
                {
                    std::vector<Mesh::face_index> borderFaces(visitedFaces.begin(), visitedFaces.end());
                    return borderFaces;
                }
            }
        }

        boost::circular_buffer<Mesh::face_index> currFaceNeighbours(tmpNeighbours.size());
        currFaceNeighbours.assign(tmpNeighbours.begin(),tmpNeighbours.end());

        Mesh::face_index currFaceOriginDsc = originFaces[currFaceDsc];
        while(*currFaceNeighbours.begin() != currFaceOriginDsc)
        {
            currFaceNeighbours.rotate(currFaceNeighbours.begin() + 1);
        }

        for (unsigned int i = 1; i < currFaceNeighbours.size(); ++i)
        {
                if(!maramiao[currFaceNeighbours[i]])
                {
                    faceDscStack.emplace(currFaceNeighbours[i]);
                    visitedFaces.insert(currFaceNeighbours[i]);
                }

            originFaces[currFaceNeighbours[i]] = currFaceDsc;
        }

        count++;
    }*/
}

std::vector<Mesh::face_index>
DirectedPartitioner::findHoles(const std::vector<bool> &visitedNodes, const std::vector<bool> &expandedNodes, const std::vector<Mesh::face_index> &outerBorder)
{
    std::vector<Mesh::face_index> facesInHoles;

    std::vector<bool> tmp(visitedNodes.size(), true);
    for(unsigned int i = 0; i < visitedNodes.size(); ++i)
    {
        if(expandedNodes[i])
            tmp[i] = false;
    }

    for(auto el : outerBorder)
        tmp[el] = false;

    for(unsigned int i = 0; i < tmp.size(); ++i)
        if(tmp[i])
        {
            facesInHoles.emplace_back(Mesh::face_index(i));
        }

    /*for(unsigned int i = 0; i < visitedNodes.size(); ++i)
    {
        if(!visitedNodes[i] && !expandedNodes[i])
            tmp[i] = true;
    }

    for(auto el : outerBorder)
        tmp[el] = false;

    for(unsigned int i = 0; i < tmp.size(); ++i)
        if(tmp[i])
            facesInHoles.emplace_back(Mesh::face_index(i));*/

    return facesInHoles;
}

void DirectedPartitioner::fillHoles(const Mesh &mesh,
                                  const std::vector<Mesh::face_index> &holesDscs,
                                  std::vector<bool> &insertedFaces,
                                  const std::vector<bool> &isNodeInSubMesh)
{
    for(auto holeFaceDsc : holesDscs)
    {
        std::stack<Mesh::face_index> holeFacesStack;

        if(!insertedFaces[holeFaceDsc])
        {
            holeFacesStack.emplace(holeFaceDsc);
            insertedFaces[holeFaceDsc] = true;
        }
        else
            continue;

        while(!holeFacesStack.empty())
        {
            Mesh::face_index currFaceDsc = holeFacesStack.top();
            holeFacesStack.pop();

            CGAL::Face_around_face_iterator<Mesh> fafBegin, fafEnd;
            for (boost::tie(fafBegin, fafEnd) = CGAL::faces_around_face(mesh.halfedge(currFaceDsc), mesh); fafBegin != fafEnd; ++fafBegin)
            {
                if(*fafBegin != Mesh::null_face() && !insertedFaces[*fafBegin] && isNodeInSubMesh[*fafBegin])
                {
                    holeFacesStack.emplace(*fafBegin);
                    insertedFaces[*fafBegin] = true;
                }
            }
        }
    }
}

void DirectedPartitioner::unravelPartition(const Mesh::face_index lastFaceDsc,
                                         const Mesh::face_index lastButOneFaceDsc,
                                         const Mesh &mesh,
                                         std::vector<bool> &isFaceExplored,
                                         const std::vector<bool> &isNodeInSubMesh)
{
    // Explore until first element of the other border is found
    std::stack<Mesh::face_index> faceDscStack;
    std::unordered_map<Mesh::face_index,Mesh::face_index> originFaces;
    std::unordered_set<Mesh::face_index> isFaceVisited;
    unsigned int numFaces = 0;
    for(auto i : isFaceExplored) if(i) numFaces++;
    bool isSenseCCW = true;

    faceDscStack.emplace(lastButOneFaceDsc);
    originFaces[lastButOneFaceDsc] = lastFaceDsc;
    isFaceExplored[lastFaceDsc] = false;
    numFaces--;

    CGAL::Face_around_face_iterator<Mesh> fafib, fafie;
    int count = 0;
    while(numFaces > isNodeInSubMesh.size()/2)
    {
        Mesh::face_index currFaceDsc = faceDscStack.top();
        faceDscStack.pop();
        isFaceExplored[currFaceDsc] = false;
        numFaces--;

        std::vector<Mesh::face_index> currFaceNeighs;
        unsigned short currNeighIdx = 0;
        short originIdx = -1;
        short nullFaceIdx = -1;
        for (boost::tie(fafib,fafie) = CGAL::faces_around_face(mesh.halfedge(currFaceDsc),mesh); fafib != fafie; ++fafib)
        {
            if(*fafib != Mesh::null_face() && isNodeInSubMesh[*fafib])
            {
                    currFaceNeighs.emplace_back(*fafib);

                    if(*fafib == originFaces[currFaceDsc])
                    {
                        originIdx = currNeighIdx;
                    }
            }
            else
            {
                nullFaceIdx = currNeighIdx;
            }

            currNeighIdx++;
        }

        if(currFaceNeighs.size() == 2)
        {
            if(isSenseCCW)
            {
                bool cond1 = originIdx == 0 && nullFaceIdx == 2;
                bool cond2 = originIdx == 1 && nullFaceIdx == 0;
                bool cond3 = originIdx == 2 && nullFaceIdx == 1;

                if(cond1 || cond2 || cond3)
                    isSenseCCW = false;
            }
            else
            {
                bool cond1 = originIdx == 0 && nullFaceIdx == 1;
                bool cond2 = originIdx == 1 && nullFaceIdx == 2;
                bool cond3 = originIdx == 2 && nullFaceIdx == 0;

                if(cond1 || cond2 || cond3)
                    isSenseCCW = true;
            }
        }

        boost::circular_buffer<Mesh::face_index> currFaceCircularNeighs(currFaceNeighs.size());
        currFaceCircularNeighs.assign(currFaceNeighs.begin(),currFaceNeighs.end());

        Mesh::face_index currFaceOriginDsc = originFaces[currFaceDsc];
        while(*currFaceCircularNeighs.begin() != currFaceOriginDsc)
        {
            currFaceCircularNeighs.rotate(currFaceCircularNeighs.begin() + 1);
        }

        if(isSenseCCW)
        {
            for(unsigned int i = 1; i < currFaceCircularNeighs.size(); ++i)
            {
                Mesh::face_index currNeigh = currFaceCircularNeighs[i];
                if(isFaceExplored[currNeigh])
                {
                    faceDscStack.emplace(currNeigh);
                    originFaces[currNeigh] = currFaceDsc;
                }
            }
        }
        else
        {
            for(unsigned int i = currFaceCircularNeighs.size() - 1; i > 0; --i)
            {
                Mesh::face_index currNeigh = currFaceCircularNeighs[i];
                if(isFaceExplored[currNeigh])
                {
                    faceDscStack.emplace(currNeigh);
                    originFaces[currNeigh] = currFaceDsc;
                }
            }
        }
    }

    std::cout << "OIIIII "<< faceDscStack.top() << std::endl;
}

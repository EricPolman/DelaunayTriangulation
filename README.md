Delaunay Triangulation
======================
This implementation of a Divide & Conquer Delaunay Triangulation was for an assignment I had to do for IGAD. I had to create a triangulation based on a randomly generated point set.
Initially, I started to do research on triangulation which quickly led me to Delaunay Triangulation. This concept is fairly easy to understand and execute in a brute force way. If you test all possible triangles against all points and check if none of those points are in the circumcircle of said triangle, it's a valid triangle. This makes bruteforcing it a O(n^4) process, for all points, you find 2 points to connect to, and check that against all points.  

I had some issues with finding an implementation of getting the circumcircle of a triangle. Lots of info is available on the radius though, but that's easy when you have the center already. As I couldn't find an implementation in code, I went to wikipedia to just use the formula they offered and translated that to code.

Now that I could find the circumcenter and it's radius (which is just the distance to one of the points of the triangle) I could setup the brute forcing algorithm. I also gave every vertex a colour, to make it look a lot better than just a triangulated mesh. The triangle takes an average colour between the three vertices. To make the image screen-filling, I added points to the corners of the window. 

While this method works, it's painfully slow. I went on to search for alternatives. There's an iteration algorithm and a divide-and-conquer algorithm. I used the latter. Googling for ways to make the process faster, I found out that currently Guibas-Stolfi is the fastest implementation. Unfortunately, the paper describing this algorithm is behind a paywall. After doing more research, I found out that Guibas-Stolfi is based on Lee-Schachter's Divide and Conquer, of which the article is available here: http://www.personal.psu.edu/cxc11/AERSP560/DELAUNEY/13_Two_algorithms_Delauney.pdf 

This article essentially describes the following process:
1. Sort the list of vertices based on their X position. 
2. Split the list in two in the middle (in my case, I make two separate meshes)
3. Triangulate both meshes
4. Find the hull of both meshes (Which I did with Graham's Scan, because of its ease of implementation and decent complexity)
5. Stitch the meshes starting from the bottom, working your way up to the top
The last part is the trickiest part. The triangulation part is easy, since I already had the code for that. 
First, I had to create the convex hulls for both meshes. As I said, I used Graham's Scan, which also sorts my hull in counter-clockwise direction. As I have non-random points for the corners, I always know the lowest and highest points of my mesh, making the creation of the hull a lot easier. I know where to start already. 

So, now I have two hulls and a linked list for each hull to loop through. I start off with a lower tangent, which are the lowest points of both hulls. I also have a leftNode and a rightNode. I check both points of the lower tangent with the left and right node. Whichever one is valid will become the triangle. When a triangle is created, the side of which most points are used (so the node and the lower-tangent vertex) will move one step forward. So the lower-tangent vertex will become what is currently the node's vertex and the node will just move one step further in the linked list. I work my way up until the lower tangent is equal to the upper tangent. That's when I know the triangulation is complete. 

There are several problems with this implementation. It's possible that a triangle is a valid Delaunay-triangle when evaluated against its own set of vertices, but invalid when evaluated against all vertices. When this happens, I have to break that triangle and insert the new hull-vertex into the linked list to evaluate triangles with that vertex. If that is also connected to an invalid triangle, that triangle will be broken down as well.
I've also tried splitting the set in four meshes, to further increase speed. This is highly unstable though, it doesn't always work, but for this assignment, I thought it was worth to at least check it out. 


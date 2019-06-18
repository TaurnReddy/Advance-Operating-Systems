# Advance-Operating-Systems
SLEEPING TEACHING ASSISTANT
A university computer science department has a teaching assistant (TA) that holds office hours where they provide help to undergraduate students with their programming assignments. The TA's office is rather small, and only has room for one desk with a chair and a computer. There are three chairs outside the office where students may sit and wait if the TA is currently helping another student. If there are no available chairs in the waiting area, the student shows up at a later time. When the TA has finished helping a student, she goes outside to the waiting area to see if there are waiting students. If so, she takes the next student and begins to help them. If there are no waiting students, she returns to her chair in he office and takes a nap. If a student shows up and sees the TA sleeping, they sit in her chair and wake her up.

Using POSIX mutex locks and sempahores, design a solution to the sleeping TA problem. A simple solution can use one mutex lock and two semaphores:

- the mutex lock for controlling access to the chairs. (This of course can be simplified by keeping track of the number of waiting students which roughly resembles the number of students sitting in chairs.)

- a semaphore for signalling the sleeping TA that a student has shown up.

- a semaphore for signalling a waiting student that the TA can now help them.

Students and the TA

Using Pthreads, begin by creating N students where each student runs as a separate thread. In addition, the TA will also run as a separate thread. Student threads will alternate between programming for a period of time, and then seeking help from the TA. If the TA is available, they will obtain help or will sit in a hallway while waiting their turn with the TA. If there are no chairs available, the student will resume programming on their own, and seek help at a later time. If a student arrives and notices the TA is sleeping, they must notify the TA using the appropriate semaphore.

If the TA is helping a student, upon finishing they must check if there are waiting students in the hallway. If so, they must help them as well. They will notify a waiting student by using the appropriate semaphore. If there are no waiting students, the TA can take a nap.

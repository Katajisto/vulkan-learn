# Milestone 1:
I started the project by following a basic youtube tutorial with C and Vulkan and GLFW. 
This went quite well for a while, until I got fed up with the slow pace of the tutorial.

This is where I decided to jump into just reading the vulkan-tutorial.com site and writing that in C.
I got pretty far down the tutorial (somewhere around swap chain creation). This is where I realized I had
absolutely no clue what I was supposed to be doing. I had all of these nice structures initialized, but no idea what I was supposed to do with them.

Then I read the lecture slides and came to the conclusion that maybe it would be better if I used VkBootstrap in order to actually make something working
so I could start understanding what the structures do. Now I used VkBootstrap with C++ and made another project. This project went well, until I again
had all of the structures but no idea what to do.

At this point I read the lecture slides again and started a new project following vkguide.dev. Only difference is that I was using GLFW, becuase
that was what I was using in the beginning and I already kind of knew how to work with it.

This went very well until the vkguide.dev tutorial needed something that was present in vulkan 1.3. Of course I only had 1.2. Now I struggled with 
updating Vulkan for a long time on linux, and when I finally got it, then I had another issue where my graphics card drivers did not seem to support 1.3

At this point I was very annoyed and decided to just boot up my dual boot windows installation and do it there. This is when I started this repository.
I copied over the tutorial code I had written and spent a long time setting up cmake on windows. Vulkan was fairly easy to install, but GLFW was very annoying to install,
since I have very little experience with C/C++ dependencies, especially on windows. When I finally got GLFW working
by just adding the .a library file to the directory, I managed to follow the tutorial to the end and get this working.

I am very confused about many things. I have been so busy solving stupid build issues that learning about Vulkan (especially semaphores and the rendering) has suffered.
I hope to spend the next 2 weeks on less build stuff and more actual learning. 

I did not go over the process of writing the code too much, since I was mostly just following the tutorial, altough I followed it quite badly and will have to refactor many things in the coming weeks.


Project: https://github.com/Katajisto/vulkan-learn

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

# Milestone 2:

I tried hard to get Vulkan 1.3 working on my mac, but this was clearly not meant to happen. It cost me a lot of time and I never got it working. Luckily, atleast I got a cleaner cmake file.

So once again I did a lot of just copying tutorial code, since I had to change the tutorial to the first one I tried, because the new one wanted to use a new vulkan version that
caused me to have so many problems. I copied very mindlessly the tutorial code up to vertex buffers in the vulkan-tutorial.com. Then after some work adding different kinds of flags, I 
got the program to actually run on my mac finally. 

Now I started work on the vertex buffers and I understood it mostly. I was able to send a buffer to the shader and draw it. I also configured CLion to run the script to compile the
shaders every time I run the program, altough I need to add a system where it fails the build if it doesn't go trough. 

Then next thing was to add some info about time to the shader. I was used to raylib that automatically provides this to the shader so I knew this was something that needs to be provided.
I set up a pipeline to send  a frame info struct thing to the shader. It currently only sends a frame number but I probably want to add some other stuff there later on. Atleast time.

Last thing I did was to add a deletion queue instead of the cleanup function. This way I can ask to delete the things the moment I create them, instead of having to go look for the cleanup function.

I think I might need another queue once we create and delete things each frame.

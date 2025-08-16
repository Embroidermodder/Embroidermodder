---
title: "Fast Forward"
author: "Jonathan"
---

There have been some changes that happened over the past several months:

The launch of the Kickstarter campaign was delayed but is still going to happen. Unless there are other unforeseen setbacks,
the revised plan is to launch our campaign on March 17.

We have started setup of a nightly build/continuous integration system using Travis-CI.
This is important as it will allow quicker bug fixes and increased stability long term. Currently
this is only available for 64-bit Linux builds but will eventually include OSX, Windows and Arduino.

We have also improved the stability and improved the API of our backend library: libembroidery. The API is
still changing but we now can produce standalone static and shared versions of libembroidery. Another notable
change is that there are now experimental Delphi and Lazarus(FreePascal) bindings in the works for libembroidery, contributed by <a href="https://github.com/x2nie">x2nie</a>.

Last but not least, I have a demonstration of our libembroidery code embedded on an Arduino. The video below simulates the stitching
by drawing it to a 2.8" TFT display (v1) made by <a href="https://adafruit.com">Adafruit</a>. It's really fast so I slowed it down for the video.
Instead of drawing, we could change it to control motors and other hardware to create an open source embroidery machine.
This is what the brain of an embroidery machine looks like and it just needs a heart and body. Enjoy the <a href="https://www.youtube.com/watch?v=KqiKfn4lxBk">video</a>!
<div class="video"><iframe src="https://www.youtube.com/embed/KqiKfn4lxBk" frameborder="0" allowfullscreen></iframe></div>

--Jonathan

<a href="news0.html#demo">Permanent link to this article</a>

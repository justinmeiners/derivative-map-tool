# Derivative Map from Height Maps

A command line tool for computing derivative maps from height maps.
Despite lots of articles describing the process, I couldn't find an actual tool to do it.
The idea for how it should work is obvious, but there are some gotchas to get it to match standard tools, such as normalizing the range of derivatives, and using the [Sobel Operator][6] to compute
higher quality derivatives.

No dependencies required besides standard library.

![heightmap][sample/heightmap.png]
![derivative map][sample/derivative.png]

## About Derivative Maps

Derivative maps are a bump mapping alternative to tangent space normal or height maps.
The key advantage is that it uses screen space derivatives so you don't 
need to compute and store model tangent space.


See [Rory][1], [Whigham][4], [Mikkelsen 1][2]
and [Mikkelsen 2][3].
[Blender][5] was also a helpful reference.


## LICENSE

[GPL 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)

[1]: http://www.rorydriscoll.com/2012/01/11/derivative-maps/
[2]: http://mmikkelsen3d.blogspot.com/2011/07/derivative-maps.html
[3]: http://mmikkelsen3d.blogspot.com/2011/11/derivative-maps-in-xnormal.html
[4]: http://johnwhigham.blogspot.com/2015/03/derivative-mapping.html
[5]: https://developer.blender.org/rBSd917bdb095573161522194449fc22f6809e5b5b2
[6]: https://en.wikipedia.org/wiki/Sobel_operator




# Online Viewer and Converter

!!! warning
    EXPERIMENTAL

<script>
  /* Call clang generated WASM here. */
  
</script>

If you only need to convert and view machine embroidery files (like our old Android application) then this page
does just that. To access other features of the Embroidermodder Project please see the [downloads.html](Downloads page).

<!-- Uses the native file dialog to get the string as a file object that is passed to a function from script above.
     If this is not called first, produce an error message. -->
<button onclick="upload();">Upload File</button>

<!-- Displays the SVG file output as a widget below. This could be by default. -->
<button onclick="show_svg();">Show</button>

<!-- Brings up the native file dialog, call "convert" with the arguments. -->
<button onclick="export_to();">Export...</button>

<svg class="viewer"></svg>

This viewer uses no cookies and no external tools, so if you save this webpage to use offline it will still function.
Eventually, this webpage can be embedded in both an Android and an iOS app so we have, in total, 3 front-ends: embroider,
embroidermodder and the viewer/converter.

// headers
using ManagedBass;
// datadef
var vRootPath = System.IO.Directory.GetCurrentDirectory();
Console.WriteLine(
    String.Format(
        "what file should i play ? (relative to {0}/)",
        vRootPath
    )
);
var vFilePath = String.Format("{0}/{1}", vRootPath, Console.ReadLine());
// actions
#if true
Bass.Init(); // Initialize with default options.
var vFileHandle = Bass.CreateStream(vFilePath);
Bass.ChannelPlay(vFileHandle);
Console.ReadKey(); // Wait till user presses a Key.
Bass.ChannelStop(vFileHandle);
Bass.Free(); // Free the device.
#endif

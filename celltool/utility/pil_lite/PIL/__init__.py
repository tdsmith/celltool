def init_for_py2exe():
  import sys, imp
  if sys.platform == 'win32' and (hasattr(sys, "frozen") or imp.is_frozen("__main__")):
    import ArgImagePlugin
    import BmpImagePlugin
    import BufrStubImagePlugin
    import CurImagePlugin
    import DcxImagePlugin
    import FitsStubImagePlugin
    import FliImagePlugin
    import GbrImagePlugin
    import GifImagePlugin
    import GribStubImagePlugin
    import Hdf5StubImagePlugin
    import IcnsImagePlugin
    import IcoImagePlugin
    import ImImagePlugin
    import ImtImagePlugin
    import IptcImagePlugin
    import JpegImagePlugin
    import McIdasImagePlugin
    import MpegImagePlugin
    import MspImagePlugin
    import PcdImagePlugin
    import PcxImagePlugin
    import PixarImagePlugin
    import PngImagePlugin
    import PpmImagePlugin
    import PsdImagePlugin
    import SgiImagePlugin
    import SpiderImagePlugin
    import SunImagePlugin
    import TgaImagePlugin
    import TiffImagePlugin
    import WmfImagePlugin
    import XVThumbImagePlugin
    import XbmImagePlugin
    import XpmImagePlugin

init_for_py2exe()
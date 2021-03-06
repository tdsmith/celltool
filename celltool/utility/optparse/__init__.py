"""optik

A powerful, extensible, and easy-to-use command-line parser for Python.

By Greg Ward <gward@python.net>

See http://optik.sourceforge.net/
"""

# Copyright (c) 2001-2006 Gregory P. Ward.  All rights reserved.
# See the README.txt distributed with Optik for licensing terms.

__revision__ = "$Id: __init__.py 526 2006-07-23 15:19:14Z greg $"

__version__ = "1.5.3"


# Re-import these for convenience
from option import Option
from option_parser import *
from help import *
from errors import *

import option, option_parser, help, errors
__all__ = (option.__all__ +
           option_parser.__all__ +
           help.__all__ +
           errors.__all__)


# Some day, there might be many Option classes.  As of Optik 1.3, the
# preferred way to instantiate Options is indirectly, via make_option(),
# which will become a factory function when there are many Option
# classes.
make_option = Option

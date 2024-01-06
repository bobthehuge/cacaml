import sys
from enum import Enum

class CML_STATUS(Enum):
    CML_INFO = '[CML_INFO]'
    CML_LOG = '[CML_LOG]'
    CML_WARN = '[CML_WARNING]'
    CML_ERR = '[CML_ERROR]'
    CML_FATAL = '[CML_FATAL]'

def cml_log(message: str, status: str=CML_STATUS.CML_LOG, end: str='\n'):
    deco = '' if len(status)==0 else ' '
    sys.stdout.write(status+deco+message+end)

def cml_err(message: str, status: str=CML_STATUS.CML_ERR, end: str='\n'):
    deco = '' if len(status)==0 else ' '
    sys.stderr.write(status+deco+message+end)

def cml_exit(message: str, errcode: int=1, end: str='\n'):
    sys.stderr.write(CML_STATUS.CML_FATAL.value+' '+message+end)
    sys.exit(errcode)

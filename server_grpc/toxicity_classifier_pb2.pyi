from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class MessageRequest(_message.Message):
    __slots__ = ("message",)
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    message: str
    def __init__(self, message: _Optional[str] = ...) -> None: ...

class MessageResponse(_message.Message):
    __slots__ = ("toxicity_probability",)
    TOXICITY_PROBABILITY_FIELD_NUMBER: _ClassVar[int]
    toxicity_probability: float
    def __init__(self, toxicity_probability: _Optional[float] = ...) -> None: ...

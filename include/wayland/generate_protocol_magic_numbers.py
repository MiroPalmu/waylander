def get_wayland_xml(wayland_xml_download_address: str = "https://gitlab.freedesktop.org/wayland/wayland/-/raw/main/protocol/wayland.xml?inline=false") -> bytes:
    import subprocess
    curl_call = subprocess.run(["curl", wayland_xml_download_address], capture_output=True)
    curl_call.check_returncode()
    return curl_call.stdout


from dataclasses import dataclass
from enum import Enum
from typing import Optional

@dataclass
class wl_description:
    # Required attributes:
    summary: str

    # #(PCDATA)
    description: str

class wl_arg_type(Enum):
    wl_int = 0
    wl_uint = 1
    wl_fixed = 2
    wl_string = 3
    wl_object = 4
    wl_new_id = 5
    wl_array = 6
    linux_fd = 7

@dataclass
class wl_arg:
    # Required attributes:
    name: str
    arg_type: wl_arg_type

    # Optional attributes:
    interface: Optional[str]
    allow_null: Optional[bool]
    enum: Optional[str]

    # (description?)
    description: Optional[wl_description]

@dataclass
class wl_enum_entry:
    # Required attributes:
    name: str
    value: int

    # Optional attributes:
    summary: Optional[str]
    since_version: Optional[int]

    # (description?)
    description: Optional[wl_description]


@dataclass
class wl_enum:
    # Required attributes
    name: str

    # Optional attributes
    since_version: Optional[int]
    is_bitfield: bool

    # (description?,entry*)
    description: Optional[wl_description]
    entries: list[wl_enum_entry]


@dataclass
class wl_event:
    # Required attributes
    name: str

    # Optional attributes
    event_type: Optional[str]
    since_version: Optional[int]

    # (description?,arg*)
    description: Optional[wl_description]
    args: list[wl_arg]

@dataclass
class wl_request:
    # Required attributes
    name: str

    # Optional attributes
    request_type: Optional[str]
    since_version: Optional[int]

    # (description?,arg*)
    description: Optional[wl_description]
    args: list[wl_arg]

@dataclass
class wl_interface:
    # Required attributes
    name: str
    version: int

    # (description?,(reqest|event|enum)+)
    description: Optional[wl_description]
    requests: list[wl_request]
    events: list[wl_event]
    enums: list[wl_enum]

@dataclass
class wl_protocol:
    # Required attributes
    name: str

    # (copyright?, description?, interface+)
    copyright: Optional[str]
    description: Optional[wl_description]
    interfaces: list[wl_interface]

from lxml import etree as ET

def parse_wl_description(node: ET.Element) -> wl_description:
    assert "description" == node.tag, f"Expected description node, got {node.tag} at line number: {node.sourceline}"
    return wl_description(node.attrib["summary"], str(node.text))


def parse_wl_arg(node: ET.Element) -> wl_arg:
    assert "arg" == node.tag, f"Expected arg node, got {node.tag} at line number: {node.sourceline}"

    parsed_desc = None
    if (len(node) == 0):
        pass
    elif (len(node) == 1):
        parsed_desc = parse_wl_description(node[0])
    else:
        raise RuntimeError("arg xml node contained more than one children")

    type_str = node.attrib["type"]
    parsed_type = None
    if (type_str == "int"):
        parsed_type = wl_arg_type.wl_int
    elif (type_str == "uint"):
        parsed_type = wl_arg_type.wl_uint
    elif (type_str == "fixed"):
        parsed_type = wl_arg_type.wl_fixed
    elif (type_str == "string"):
        parsed_type = wl_arg_type.wl_string
    elif (type_str == "object"):
        parsed_type = wl_arg_type.wl_object
    elif (type_str == "new_id"):
        parsed_type = wl_arg_type.wl_new_id
    elif (type_str == "array"):
        parsed_type = wl_arg_type.wl_array
    elif (type_str == "fd"):
        parsed_type = wl_arg_type.linux_fd
    else:
        raise RuntimeError("could not parse type of wl_arg")

    allow_null_txt = node.attrib.get("allow-null", "false")

    return wl_arg(
        name = node.attrib["name"],
        arg_type = parsed_type,
        interface = node.attrib.get("interface"),
        allow_null = (allow_null_txt == "true"),
        enum = node.attrib.get("enum"),
        description = parsed_desc,
    )

def parse_wl_enum_entry(node: ET.Element) -> wl_enum_entry:
    assert "entry" == node.tag, f"Expected entry node, got {node.tag} at line number: {node.sourceline}"

    parsed_desc = None
    if (len(node) == 0):
        pass
    elif (len(node) == 1):
        parsed_desc = parse_wl_description(node[0])
    else:
        raise RuntimeError("entry xml node contained more than one children")

    since_version = node.attrib.get("since")
    parsed_since_version = None

    if since_version is not None:
        parsed_since_version = int(since_version)

    return wl_enum_entry(
        name = node.attrib["name"],
        value = int(node.attrib["value"]),
        summary = node.attrib.get("summary"),
        since_version = parsed_since_version,
        description = parsed_desc
    )

def parse_wl_enum(node: ET.Element) -> wl_enum:
    assert "enum" == node.tag, f"Expected enum node, got {node.tag} at line number: {node.sourceline}"

    since_version = node.attrib.get("since")
    parsed_since_version = None
    if since_version is not None:
        parsed_since_version = int(since_version)

    parsed_desc = None
    if (len(node) == 0):
        pass
    elif (len(node) == 1):
        parsed_desc = parse_wl_description(node[0])
    else:
        raise RuntimeError("enum xml node contained more than one children")

    potential_entries = node[int(parsed_desc is not None):]
    parsed_entries = [parse_wl_enum_entry(child) for child in potential_entries]

    return wl_enum(
        name = node.attrib["name"],
        since_version = parsed_since_version,
        is_bitfield = (node.attrib.get("bitfield", "false") == "true"),
        description = parsed_desc,
        entries = parsed_entries
    )

def parse_wl_event(node: ET.Element) -> wl_event:
    assert "event" == node.tag, f"Expected event node, got {node.tag} at line number: {node.sourceline}"

    parsed_event_type = None
    if "type" in node.attrib:
        parsed_event_type = node.attrib["type"]

    since_version = node.attrib.get("since")
    parsed_since_version = None
    if since_version is not None:
        parsed_since_version = int(since_version)

    parsed_desc = None
    if (len(node) >= 1):
        parsed_desc = parse_wl_description(node[0])

    potential_args = node[int(parsed_desc is not None):]
    parsed_args = [parse_wl_arg(child) for child in potential_args]

    return wl_event(
        name = node.attrib["name"],
        event_type = parsed_event_type,
        since_version = parsed_since_version,
        description = parsed_desc,
        args = parsed_args
    )

def parse_wl_request(node: ET.Element) -> wl_request:
    assert "request" == node.tag, f"Expected request node, got {node.tag} at line number: {node.sourceline}"

    parsed_request_type = None
    if "type" in node.attrib:
        parsed_request_type = node.attrib["type"]

    since_version = node.attrib.get("since")
    parsed_since_version = None
    if since_version is not None:
        parsed_since_version = int(since_version)

    parsed_desc = None
    if (len(node) >= 1):
        parsed_desc = parse_wl_description(node[0])

    potential_args = node[int(parsed_desc is not None):]
    parsed_args = [parse_wl_arg(child) for child in potential_args]

    return wl_request(
        name = node.attrib["name"],
        request_type = parsed_request_type,
        since_version = parsed_since_version,
        description = parsed_desc,
        args = parsed_args
    )

def parse_wl_interface(node: ET.Element) -> wl_interface:
    assert "interface" == node.tag, f"Expected interface node, got {node.tag} at line number: {node.sourceline}"

    if (len(node) <= 1):
        raise RuntimeError("interface xml node contains no children")

    parsed_desc = None
    if (node[0].tag == "description"):
        parsed_desc = parse_wl_description(node[0])

    non_desc_children = [child for child in node[int(parsed_desc is not None):]]
    parsed_requests: list[wl_request] = []
    parsed_events: list[wl_event] = []
    parsed_enums: list[wl_enum] = []

    for child in non_desc_children:
        if child.tag == "request":
            parsed_requests.append(parse_wl_request(child))
        elif child.tag == "event":
            parsed_events.append(parse_wl_event(child))
        elif child.tag == "enum":
            parsed_enums.append(parse_wl_enum(child))

    return wl_interface(
        name = node.attrib["name"],
        version = int(node.attrib["version"]),
        description = parsed_desc,
        requests = parsed_requests,
        events = parsed_events,
        enums = parsed_enums
    )

def parse_wl_protocol(node: ET.Element) -> wl_protocol:
    assert "protocol" == node.tag, f"Expected protocol node, got {node.tag} at line number: {node.sourceline}"

    if (len(node) == 0):
        raise RuntimeError("protocol xml node contains no children")

    parsed_copyright = None
    if (node[0].tag == "copyright"):
        parsed_copyright = node[0].text

    parsed_desc = None
    if (len(node) > 1):
        index_of_desc = int(parsed_copyright is not None)
        if (node[index_of_desc].tag == "description"):
            parsed_desc = parse_wl_description(node[index_of_desc])

    amount_of_non_interface_children = int(parsed_copyright is not None) + int(parsed_desc is not None)
    parsed_interfaces = [parse_wl_interface(x) for x in node[amount_of_non_interface_children:]]

    return wl_protocol(
        name = node.attrib["name"],
        copyright = parsed_copyright,
        description = parsed_desc,
        interfaces = parsed_interfaces
    )

def parse_wayland_xml(xml: bytes):
    root = ET.fromstring(xml)
    return parse_wl_protocol(root)

if __name__ == "__main__":
    test_address = "https://gitlab.freedesktop.org/wayland/wayland/-/raw/main/protocol/tests.xml?ref_type=heads&inline=false"
    print(parse_wayland_xml(get_wayland_xml(test_address)))

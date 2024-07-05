"""
Generate C++ header from xml in Wayland xml format.

As MVP currently ignores many optional attributes.
"""


def get_wayland_xml(wayland_xml_download_address: str = "https://gitlab.freedesktop.org/wayland/wayland/-/raw/main/protocol/wayland.xml?inline=false") -> bytes:
    import subprocess
    curl_call = subprocess.run(["curl", wayland_xml_download_address], capture_output=True)
    curl_call.check_returncode()
    return curl_call.stdout

from dataclasses import dataclass
from enum import Enum
from typing import Optional

reserved_identifiers = set(["default"])

class wl_arg_type(Enum):
    wl_int = 0
    wl_uint = 1
    wl_fixed = 2
    wl_string = 3
    wl_object = 4
    wl_new_id = 5
    wl_array = 6
    linux_fd = 7

def get_cxx_Wtype(type: wl_arg_type, interface: Optional[str] = None) -> str:

    if type == wl_arg_type.wl_int:
        return "Wint"
    elif type == wl_arg_type.wl_uint:
        return "Wuint"
    elif type == wl_arg_type.wl_fixed:
        return "Wfixed"
    elif type == wl_arg_type.wl_string:
        return "Wstring"
    elif type == wl_arg_type.wl_object:
        return f"Wobject<{interface if interface else ''}>"
    elif type == wl_arg_type.wl_new_id:
        return f"Wnew_id<{interface if interface else ''}>"
    elif type == wl_arg_type.wl_array:
        return "Warray"
    elif type == wl_arg_type.linux_fd:
        return "Wfd"
    else:
       raise RuntimeError(f"Unregonized wl_ar_type: {type}")

def commentify(s: str, comment_symbol: str = "///") -> str:
    comm = ""
    lines = s.split("\n")
    skip_first: bool = len(lines[0].lstrip()) == 0
    skip_last: bool = len(lines[-1].lstrip()) == 0
    for line in lines[int(skip_first):len(lines) - int(skip_last)]:
        comm += comment_symbol
        stripped_line = line.lstrip()
        if len(stripped_line) != 0:
            comm += " " + stripped_line
        comm += "\n"
    return comm

@dataclass
class wl_description:
    # Required attributes:
    summary: str

    # #(PCDATA)
    description: Optional[str]

    def as_sphinx_comment(self, summary_prefix: str = "", indent_in_spaces: int = 0) -> str:
        indent = ' ' * indent_in_spaces
        header = indent + f"/// {summary_prefix}{self.summary}\n"
        if not self.description:
            return header
        spacer = indent + "///\n"
        body = commentify(self.description, comment_symbol = indent + "///")
        return header + spacer + body


@dataclass
class wl_arg:
    # Required attributes:
    name: str
    arg_type: wl_arg_type

    # Optional attributes:
    summary: Optional[str]
    interface: Optional[str]
    allow_null: Optional[bool]
    enum: Optional[str]

    # (description?)
    description: Optional[wl_description]

    def as_cxx_data_member(self, interface: str, indent_in_spaces: int = 4) -> str:

        desc = ""
        indent = ' ' * indent_in_spaces
        if self.summary and not self.description:
            desc = indent + f"/// {self.summary}\n"

        if self.description:
            if self.summary:
                desc += indent + "///\n"
            desc = self.description.as_sphinx_comment(indent_in_spaces = indent_in_spaces)
            desc += "\n"

        indent = ' ' * indent_in_spaces

        if self.enum:
            # Apparently format is [interface.]enum
            if "." in self.enum:
                cxx_type = self.enum.replace(".", "::")
            else:
                cxx_type = interface + "::" + self.enum
        else:
            cxx_type = get_cxx_Wtype(self.arg_type, interface = self.interface)

        return f"{desc}{indent}{cxx_type} {self.name};\n"

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

    def as_cxx_enum_class_entry(self, indent_in_spaces: int = 4, last = False) -> str:
        desc = ""
        indent = ' ' * indent_in_spaces
        if self.summary and not self.description:
            desc = indent + f"/// {self.summary}\n"

        if self.description:
            if self.summary:
                desc += indent + "///\n"
            desc = self.description.as_sphinx_comment(indent_in_spaces = indent_in_spaces)
            desc += "\n"


        indent = ' ' * indent_in_spaces
        c = "" if last else ","
        name = "E" + self.name
        return desc + indent + f"{name} = {self.value}{c}\n"

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

    def underlying_type(self) -> str:
        wl_underlying_type = wl_arg_type.wl_uint if self.is_bitfield else wl_arg_type.wl_int
        return get_cxx_Wtype(wl_underlying_type) + "::integral_type"


    def as_cxx_enum_class_decleration(self, indent_in_spaces: int = 4) -> str:
        indent = " " * indent_in_spaces
        return f"{indent}enum class {self.name} : {self.underlying_type()};\n"

    def as_cxx_enum_class_definition(self, interface: str,  indent_in_spaces: int = 4) -> str:
        indent = " " * indent_in_spaces
        sphinx_comment = ""
        if self.description:
            sphinx_comment = self.description.as_sphinx_comment(indent_in_spaces = indent_in_spaces)

        header = f"{indent}enum class {interface}::{self.name} : {self.underlying_type()} {{\n"
        body = ""
        if len(self.entries) == 0:
            raise RuntimeError(f"wl_enum {self.name} has no entries!")
        for entry in self.entries[:-1]:
            body += entry.as_cxx_enum_class_entry(indent_in_spaces = 4 + indent_in_spaces)
        body += self.entries[-1].as_cxx_enum_class_entry(indent_in_spaces = 4 + indent_in_spaces, last = True)
        tail = indent + "};\n\n"
        return sphinx_comment + header + body + tail

def cxx_opcode_static_member(interface: str, opcode: int, indent_in_spaces: int = 4):
    indent = (" " * indent_in_spaces)
    return indent + f"static constexpr Wopcode<{interface}> opcode{{ {opcode} }};\n"

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

    def as_cxx_member_struct_decleration(self, indent_in_spaces = 4) -> str:
        indent = " " * indent_in_spaces
        return indent + f"struct {self.name};\n"

    def as_cxx_member_struct_definition(self, interface: str, opcode: int) -> str:
        comment = ""
        if self.description:
            comment = self.description.as_sphinx_comment()
        header = f"struct {interface}::event::{self.name} {{\n"
        body = cxx_opcode_static_member(interface, opcode)
        for arg in self.args:
            body += arg.as_cxx_data_member(interface)
        tail = "};\n\n"
        return comment + header + body + tail

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

    def as_cxx_member_struct_decleration(self, indent_in_spaces = 4) -> str:
        indent = " " * indent_in_spaces
        return indent + f"struct {self.name};\n"

    def as_cxx_member_struct_definition(self, interface: str, opcode: int) -> str:
        comment = ""
        if self.description:
            comment = self.description.as_sphinx_comment()
        header = f"struct {interface}::request::{self.name} {{\n"
        body = cxx_opcode_static_member(interface, opcode)
        for arg in self.args:
            body += arg.as_cxx_data_member(interface)
        tail = "};\n\n"
        return comment + header + body + tail


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

    request_opcodes: list[int]
    event_opcodes: list[int]

    def has_enums(self) -> bool:
        return len(self.enums) != 0

    def as_cxx_struct_without_definitions(self) -> str:
        header = f"struct {self.name} {{\n"
        body = ""

        indent_in_spaces = 4
        indent = ' ' * indent_in_spaces

        for enum in self.enums:
            body += enum.as_cxx_enum_class_decleration(indent_in_spaces)

        if len(self.enums) != 0:
            body += "\n"

        body += indent + "struct request {\n"
        for request in self.requests:
            body += request.as_cxx_member_struct_decleration(2 * indent_in_spaces)
        body += indent + "};\n\n"

        body += indent + "struct event {\n"
        for event in self.events:
            body += event.as_cxx_member_struct_decleration(2 * indent_in_spaces)
        body += indent + "};\n"

        tail = "};\n"
        return header + body + tail

    def as_cxx_definitions(self) -> str:
        body = ""

        for enum in self.enums:
            body += enum.as_cxx_enum_class_definition(self.name, indent_in_spaces = 0)

        for opcode, request in zip(self.request_opcodes, self.requests):
            body += request.as_cxx_member_struct_definition(self.name, opcode)

        for opcode, event in zip(self.event_opcodes, self.events):
            body += event.as_cxx_member_struct_definition(self.name, opcode)

        return body

@dataclass
class wl_protocol:
    # Required attributes
    name: str

    # (copyright?, description?, interface+)
    copyright: Optional[str]
    description: Optional[wl_description]
    interfaces: list[wl_interface]

    def write_cxx_header(self, path_prefix: str = ""):
        content: str = f"// Generated from Wayland xml protocol: {self.name}\n\n"

        if self.description:
            content += self.description.as_sphinx_comment("@file\n/// ") + "\n"
        else:
            content += f"/// @file\n/// {self.name} xml protocol\n\n"

        if self.copyright:
            content += commentify(self.copyright) + "\n"

        content += "#pragma once\n\n"

        content += '#include "wayland/protocol_primitives.hpp"\n\n'

        content += "namespace ger {\n"
        content += "namespace wl {\n"
        content += "namespace protocols {\n\n"

        if len(self.interfaces) == 0:
            raise RuntimeError(f"Protocol {self.name} has no interfaces!")

        content += "/// Declare everything before they might be used.\n\n"

        for interface in self.interfaces:
            content += interface.as_cxx_struct_without_definitions()

        for interface in self.interfaces:
            content += interface.as_cxx_definitions()

        content += "} // namespace protocols\n"
        content += "} // namespace wl\n"
        content += "} // namespace ger\n"

        filename = f"{path_prefix}{self.name}_protocol.hpp"
        with open(filename, "w") as file:
            file.write(content)
        print(f"{filename} created!")

from lxml import etree as ET

def parse_wl_description(node: ET.Element) -> wl_description:
    assert "description" == node.tag, f"Expected description node, got {node.tag} at line number: {node.sourceline}"
    return wl_description(node.attrib["summary"], node.text)


def parse_wl_arg(node: ET.Element) -> list[wl_arg]:
    """
    This has to return a list of args, as new_id without interface from xml attribute 
    is preceded by a string specifying the interface name, and a uint
    specifying the version.
    """
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
    preceeding_args = []

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
        if not node.attrib.get("interface"):
            preceeding_args.append(wl_arg(
                name = "new_id_interface",
                arg_type = wl_arg_type.wl_string,
                summary = "Interface name, e.g. from registry::global event, for following Wnew_id<>.",
                interface = None,
                allow_null = None,
                enum = None,
                description = None,
            ))
            preceeding_args.append(wl_arg(
                name = "new_id_interface_version",
                arg_type = wl_arg_type.wl_uint,
                summary = "Interface version, e.g. from registry::global event, for following Wnew_id<>.",
                interface = None,
                allow_null = None,
                enum = None,
                description = None,
            ))
    elif (type_str == "array"):
        parsed_type = wl_arg_type.wl_array
    elif (type_str == "fd"):
        parsed_type = wl_arg_type.linux_fd
    else:
        raise RuntimeError("could not parse type of wl_arg")

    allow_null_txt = node.attrib.get("allow-null", "false")

    return preceeding_args + [wl_arg(
        name = node.attrib["name"],
        arg_type = parsed_type,
        summary = node.attrib.get("summary"),
        interface = node.attrib.get("interface"),
        allow_null = (allow_null_txt == "true"),
        enum = node.attrib.get("enum"),
        description = parsed_desc,
    )]

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
        value = int(node.attrib["value"], 0), # 0 is for automatix hex and decimal distingushing
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
    if len(node) >= 1 and node[0].tag == "description":
        parsed_desc = parse_wl_description(node[0])

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
    parsed_args = [
        arg
        for child in potential_args
        for arg in parse_wl_arg(child)
    ]

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
    parsed_args = [
        arg
        for child in potential_args
        for arg in parse_wl_arg(child)
    ]

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

    request_opcode_counter = 0
    event_opcode_counter = 0

    found_request_opcodes: list[int] = []
    found_event_opcodes: list[int] = []
    for child in non_desc_children:
        if child.tag == "request":
            parsed_requests.append(parse_wl_request(child))
            found_request_opcodes.append(request_opcode_counter)
            request_opcode_counter += 1
        elif child.tag == "event":
            parsed_events.append(parse_wl_event(child))
            found_event_opcodes.append(event_opcode_counter)
            event_opcode_counter += 1
        elif child.tag == "enum":
            parsed_enums.append(parse_wl_enum(child))

    return wl_interface(
        name = node.attrib["name"],
        version = int(node.attrib["version"]),
        description = parsed_desc,
        requests = parsed_requests,
        events = parsed_events,
        enums = parsed_enums,
        event_opcodes = found_event_opcodes,
        request_opcodes = found_request_opcodes,
    )

def parse_wl_protocol(node: ET.Element) -> wl_protocol:
    assert "protocol" == node.tag, f"Expected protocol node, got {node.tag} at line number: {node.sourceline}"

    if (len(node) == 0):
        raise RuntimeError("protocol xml node contains no children")

    parsed_copyright = None
    if (node[0].tag == "copyright"):
        parsed_copyright = node[0].text

    parsed_desc = None
    # Possible description is at index 0 if there is no copyright,
    # or at index 1 if there is copyright
    index_of_desc = int(parsed_copyright is not None)
    if (index_of_desc < len(node) and node[index_of_desc].tag == "description"):
        parsed_desc = parse_wl_description(node[index_of_desc])

    amount_of_non_interface_children = int(parsed_copyright is not None) + int(parsed_desc is not None)
    parsed_interfaces = [parse_wl_interface(x) for x in node[amount_of_non_interface_children:]]

    protocol_name = node.attrib["name"]
    return wl_protocol(
        name = protocol_name,
        copyright = parsed_copyright,
        description = parsed_desc,
        interfaces = parsed_interfaces
    )

def remove_comment_nodes(root: ET.Element) -> None:
    comments = root.xpath('//comment()')
    for c in comments:
        p = c.getparent()
        p.remove(c)

def parse_wayland_xml(xml: bytes):
    root = ET.fromstring(xml)
    remove_comment_nodes(root)
    return parse_wl_protocol(root)

def main(argv0: str, argv_rest: list[str], temp_dir: str):
    import argparse
    parser  = argparse.ArgumentParser(
                prog=argv0,
        description="""    - clones wayland-protocols repo to "${TMPDIR:-/tmp}"
    - parses xml from the repo
    - generates Guilander protocol headers

    If $TMPDIR is not usable defaults to /tmp""",
                exit_on_error=True,
                formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "-p",
        type=str,
        action='append',
        metavar="core|[un]stable|staging",
        choices=["core", "stable", "staging", "unstable"],
        help="""Protocols to generate. Can be given multiple times.""",
        required=True,
    )

    parser.add_argument(
        "destination-dir",
        type=str,
        help="Path of a destination directory for the generated Guilander headers.",
    )

    parser.add_argument(
        "--alt-wayland-address",
        metavar="address",
        type=str,
        default="https://gitlab.freedesktop.org/wayland/wayland.git",
        help="Alternative address to clone wayland from."
    )

    parser.add_argument(
        "--alt-wayland-protocols-address",
        metavar="address",
        type=str,
        default="https://gitlab.freedesktop.org/wayland/wayland-protocols.git",
        help="Alternative address to clone wayland-protocols from."
    )

    cmd_args = vars(parser.parse_args(argv_rest))
    protocols = set(p for p in cmd_args["p"])
    dst_dir_path = cmd_args["destination-dir"]

    wl_address = cmd_args["alt_wayland_address"]
    wl_protocols_address = cmd_args["alt_wayland_protocols_address"]

    import git
    def parse_protocols_from_repo(git_pathspec: str, repo: git.Repo):
        tree_at_head = repo.head.commit.tree
        for path_in_repo in repo.git.ls_files([git_pathspec]).splitlines():
            print(f"found: {path_in_repo}")
            xml = tree_at_head[path_in_repo].data_stream.read()
            parse_wayland_xml(xml).write_cxx_header(path_prefix=dst_dir_path + "/")

    import tempfile
    with tempfile.TemporaryDirectory(dir=temp_dir) as temp_dir:
        if "core" in protocols:
            wl_repo = git.Repo.clone_from(wl_address, temp_dir + "/wayland")
            parse_protocols_from_repo("protocol/wayland.xml", wl_repo)

        non_core_protocols = set(p for p in protocols if p != "core")
        if len(non_core_protocols) != 0:
            wl_protocols_repo = git.Repo.clone_from(wl_protocols_address, temp_dir + "/wayland-protocols")
            for protocol in non_core_protocols:
                parse_protocols_from_repo(f"{protocol}/*.xml", wl_protocols_repo)

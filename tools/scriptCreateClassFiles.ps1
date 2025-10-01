param(
    [Parameter(Mandatory=$true)]
    [string]$ClassName
)

function ToUpperSnakeCase($name) {
    $result = ""
    for ($i = 0; $i -lt $name.Length; $i++) {
        $char = $name[$i]
        
        if ($i -gt 0) {
            $prev = $name[$i - 1]
            $next = if ($i + 1 -lt $name.Length) { $name[$i + 1] } else { '' }

            # Add underscore if:
            # 1. previous is lowercase or digit and current is uppercase
            # OR
            # 2. previous is uppercase and current is uppercase and next is lowercase
            if ((($prev -ge 'a' -and $prev -le 'z') -or ($prev -ge '0' -and $prev -le '9')) -and ($char -ge 'A' -and $char -le 'Z')) {
                $result += "_"
            }
            elseif (($prev -ge 'A' -and $prev -le 'Z') -and ($char -ge 'A' -and $char -le 'Z') -and ($next -ge 'a' -and $next -le 'z')) {
                $result += "_"
            }
        }

        $result += $char
    }

    return $result.ToUpper() + "_H"
}
$IncludeGuard = ToUpperSnakeCase $ClassName
$Destination = Join-Path (Get-Location) "$ClassName.h"  # same folder as script

# Header content
$Content = @"
#pragma once
#ifndef $IncludeGuard
#define $IncludeGuard



class $ClassName {
public:
    $ClassName() = default;
    ~$ClassName() = default;

protected:

private:

};

#endif // $IncludeGuard
"@


# Write to file
$Content | Set-Content $Destination -Encoding UTF8

Write-Host "Created header: $Destination with guard $IncludeGuard"

$Contentcpp = @"
#include "$ClassName.h"
"@


$Destinationcpp = Join-Path (Get-Location) "$ClassName.cpp"  # same folder as script

$Contentcpp | Set-Content $Destinationcpp -Encoding UTF8